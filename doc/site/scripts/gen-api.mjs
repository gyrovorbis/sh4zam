#!/usr/bin/env node
// API reference generator.
//
//   node scripts/gen-api.mjs           regenerate everything
//   node scripts/gen-api.mjs --check   fail if committed output differs
//   node scripts/gen-api.mjs --no-doxygen   reuse build/xml from a previous run
//
// Input:  Doxygen XML (scripts/Doxyfile.xml -> build/xml) plus the raw headers in
//         ../../ (the SH4ZAM repo this site lives in) for signatures, back-end detection, type-generic and C++ mapping.
// Output: src/content/docs/api/**, src/content/docs/cheatsheet.md, public/api/*.json,
//         public/_redirects. Reports in build/ (undocumented.txt, unresolved-refs.txt).

import fs from 'node:fs';
import path from 'node:path';
import { execFileSync, spawnSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import { XMLParser } from 'fast-xml-parser';
import { MODULES, SUFFIXES, VERBS, CPP_CLASSES } from './data/modules.mjs';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const SHZ = path.resolve(ROOT, '../..');
const INC = path.join(SHZ, 'include/sh4zam');
const XML_DIR = path.join(ROOT, 'build/xml');
const SITE = 'https://sh4zam.com';
const UPSTREAM = 'https://github.com/gyrovorbis/sh4zam';
const args = new Set(process.argv.slice(2));
const CHECK = args.has('--check');

// ---------------------------------------------------------------------------
// Upstream metadata

const git = (...a) => execFileSync('git', ['-C', SHZ, ...a], { encoding: 'utf8' }).trim();
const COMMIT = git('rev-parse', 'HEAD');
const COMMIT_DATE = git('log', '-1', '--format=%cs');
const versionSrc = fs.readFileSync(path.join(INC, 'shz_version.h'), 'utf8');
const vfield = (f) => versionSrc.match(new RegExp(`#define\\s+SHZ_VERSION_${f}\\s+(\\d+)`))[1];
const VERSION = `${vfield('MAJOR')}.${vfield('MINOR')}.${vfield('PATCH')}`;
const srcUrl = (file, line) => `${UPSTREAM}/blob/${COMMIT}/include/sh4zam/${file}#L${line}`;

// ---------------------------------------------------------------------------
// Doxygen

function findDoxygen() {
	if (process.env.DOXYGEN) return process.env.DOXYGEN;
	const win = 'C:/Program Files/doxygen/bin/doxygen.exe';
	if (process.platform === 'win32' && fs.existsSync(win)) return win;
	return 'doxygen';
}

if (!args.has('--no-doxygen')) {
	fs.rmSync(XML_DIR, { recursive: true, force: true });
	fs.mkdirSync(XML_DIR, { recursive: true });
	const res = spawnSync(findDoxygen(), ['scripts/Doxyfile.xml'], { cwd: ROOT, encoding: 'utf8' });
	if (res.error) throw new Error(`doxygen failed to start (${res.error.message}). Install Doxygen 1.18 or set DOXYGEN.`);
	// Upstream's Doxyfile carries a few tags newer Doxygen versions call obsolete; hide that noise.
	const noise = /has become obsolete|doxygen -u/;
	const err = (res.stderr || '').split('\n').filter((l) => l.trim() && !noise.test(l));
	if (err.length) console.error(err.join('\n'));
	if (res.status !== 0) process.exit(res.status);
}

const parser = new XMLParser({
	preserveOrder: true,
	ignoreAttributes: false,
	attributeNamePrefix: '',
	trimValues: false,
	parseTagValue: false,
	parseAttributeValue: false,
	processEntities: true,
});
const xmlCache = new Map();
function loadXml(id) {
	if (!xmlCache.has(id)) {
		const file = path.join(XML_DIR, `${id}.xml`);
		xmlCache.set(id, fs.existsSync(file) ? parser.parse(fs.readFileSync(file, 'utf8')) : null);
	}
	return xmlCache.get(id);
}

const tagOf = (n) => (n && typeof n === 'object' ? Object.keys(n).find((k) => k !== ':@') : undefined);
const kids = (n) => (n && n[tagOf(n)]) || [];
const attr = (n, a) => n?.[':@']?.[a];
const isText = (n) => tagOf(n) === '#text';
const child = (n, tag) => kids(n).find((c) => tagOf(c) === tag);
const children = (n, tag) => kids(n).filter((c) => tagOf(c) === tag);
function plain(n) {
	if (Array.isArray(n)) return n.map(plain).join('');
	if (isText(n)) return String(n['#text']);
	if (tagOf(n) === 'sp') return ' ';
	return kids(n).map(plain).join('');
}
const squash = (s) => s.replace(/\s+/g, ' ').trim();
function compounddef(id) {
	const doc = loadXml(id);
	const root = doc?.find((n) => tagOf(n) === 'doxygen');
	return root && child(root, 'compounddef');
}

// ---------------------------------------------------------------------------
// Header source helpers

const srcCache = new Map();
function srcLines(file) {
	const abs = path.isAbsolute(file) ? file : path.join(ROOT, file);
	if (!srcCache.has(abs)) srcCache.set(abs, fs.readFileSync(abs, 'utf8').replace(/\r\n/g, '\n').split('\n'));
	return srcCache.get(abs);
}
const baseName = (f) => path.basename(f);

// Raw declaration text starting at `line` (1-based), up to the terminating `;` or body `{`.
function rawDeclaration(file, line, name) {
	const lines = srcLines(file);
	let out = '';
	let depth = 0;
	let started = false;
	for (let i = line - 1; i < lines.length && i < line + 40; i++) {
		let l = lines[i].replace(/\/\/.*$/, '');
		if (!started) {
			const at = l.indexOf(name);
			if (at < 0 && i === line - 1) {
				// Return type on a previous line is rare; fall back to the whole line.
			}
			started = true;
		}
		for (const ch of l) {
			if (ch === '(') depth++;
			if (ch === ')') depth--;
			if (depth === 0 && (ch === ';' || ch === '{')) {
				out += l.slice(0, l.indexOf(ch));
				return squash(out.replace(/\/\*.*?\*\//g, ''));
			}
		}
		out += l + ' ';
	}
	return squash(out);
}

// Text of the brace-delimited body following the declaration at `line`, or '' if none.
function bodyAfter(file, line) {
	const lines = srcLines(file);
	const text = lines.slice(line - 1, line + 400).join('\n');
	let depth = 0;
	let i = 0;
	for (; i < text.length; i++) {
		const ch = text[i];
		if (ch === '(') depth++;
		else if (ch === ')') depth--;
		else if (depth === 0 && ch === ';') return '';
		else if (depth === 0 && ch === '{') break;
	}
	if (i >= text.length) return '';
	let b = 0;
	for (let j = i; j < text.length; j++) {
		if (text[j] === '{') b++;
		else if (text[j] === '}' && --b === 0) return text.slice(i, j + 1);
	}
	return text.slice(i);
}

// Struct/typedef block between two lines, comments stripped of doc markers.
function rawBlock(file, start, end) {
	return srcLines(file)
		.slice(start - 1, end)
		.join('\n')
		.replace(/\s+$/gm, '');
}

// ---------------------------------------------------------------------------
// Back-end detection

function scanDir(dir, re) {
	const set = new Set();
	if (!fs.existsSync(dir)) return set;
	for (const f of fs.readdirSync(dir).sort()) {
		const p = path.join(dir, f);
		if (fs.statSync(p).isDirectory()) continue;
		const s = fs.readFileSync(p, 'utf8');
		for (const m of s.matchAll(re)) set.add(m[1]);
	}
	return set;
}
const SH4_IMPL = new Set([
	...scanDir(path.join(INC, 'inline/sh4'), /\b(shz_\w+?)_sh4\s*\(/g),
	...scanDir(path.join(SHZ, 'source/sh4'), /\b(shz_\w+?)_sh4\s*\(/g),
	...scanDir(path.join(SHZ, 'source/sh4'), /\.globl\s+_(shz_\w+)/g),
]);
const SPU_IMPL = scanDir(path.join(INC, 'inline/spu'), /\b(shz_\w+?)_spu\s*\(/g);

// ---------------------------------------------------------------------------
// Type-generic forms (shz_vector.h `_Generic` blocks, plus the C++ overload branch)

const genericOf = new Map(); // concrete -> generic
const genericTable = new Map(); // generic -> Map(ctype -> concrete)
{
	for (const f of fs.readdirSync(INC).filter((f) => f.endsWith('.h')).sort()) {
		const s = fs.readFileSync(path.join(INC, f), 'utf8').replace(/\r\n/g, '\n');
		const re = /#\s*define\s+(shz_\w+)\s*\([^)]*\)\s*\\\s*\n\s*_Generic\(([\s\S]*?)\)\s*\(/g;
		for (const m of s.matchAll(re)) {
			const gen = m[1];
			const tab = genericTable.get(gen) || new Map();
			for (const e of m[2].matchAll(/(shz_\w+_t)\s*:\s*(shz_\w+)/g)) {
				tab.set(e[1], e[2]);
				if (!genericOf.has(e[2])) genericOf.set(e[2], gen);
			}
			genericTable.set(gen, tab);
		}
		const cpp = /SHZ_INLINE\s+[\w\s*]+?\b(shz_vec_\w+)\((shz_\w+_t)\b[^)]*\)[^{]*\{\s*return\s+(shz_\w+)\(/g;
		for (const m of s.matchAll(cpp)) {
			const tab = genericTable.get(m[1]) || new Map();
			if (!tab.has(m[2])) tab.set(m[2], m[3]);
			genericTable.set(m[1], tab);
			if (!genericOf.has(m[3])) genericOf.set(m[3], m[1]);
		}
	}
}

// ---------------------------------------------------------------------------
// Doc conversion: Doxygen XML description -> Markdown

const ENTITIES = {
	ndash: '--', mdash: '--', nbsp: ' ', lsquo: "'", rsquo: "'", ldquo: '"', rdquo: '"', apos: "'", quot: '"',
	times: '×', deg: '°', copy: '(c)', trade: '(tm)', reg: '(R)', hellip: '...', laquo: '«', raquo: '»',
	larr: '<-', rarr: '->', le: '<=', ge: '>=', ne: '!=', plusmn: '±', middot: '·', pi: 'π', theta: 'θ',
	alpha: 'α', beta: 'β', gamma: 'γ', delta: 'δ', epsilon: 'ε', lambda: 'λ', mu: 'μ', sigma: 'σ', phi: 'φ', omega: 'ω',
	sup2: '²', sup3: '³', frac12: '½', frac14: '¼', sdot: '·', minus: '-', infin: '∞', sum: '∑', radic: '√',
};
const esc = (s) => s.replace(/\\/g, '\\\\').replace(/([*\[\]<`])/g, '\\$1');

const symbolsByName = new Map(); // filled before rendering
const unresolved = new Map(); // name -> Set(where)
let renderCtx = '';

function symbolLink(name, label = name) {
	const sym = symbolsByName.get(name);
	if (!sym) return null;
	return `[\`${label}\`](${sym.path})`;
}

function noteUnresolved(name) {
	if (!unresolved.has(name)) unresolved.set(name, new Set());
	unresolved.get(name).add(renderCtx);
}

// Turn bare `foo()` mentions in plain text into code or links.
function linkifyText(t) {
	return t.replace(/\b((?:shz|SHZ)_[A-Za-z0-9_]+)(\(\))?/g, (m, name, call) => {
		const label = name + (call || '');
		const link = symbolLink(name, label);
		if (link) return link;
		if (call) noteUnresolved(name);
		return `\`${label}\``;
	});
}

function inline(nodes, state) {
	let out = '';
	let skip = 0; // characters already consumed from the next text node
	for (let i = 0; i < nodes.length; i++) {
		const n = nodes[i];
		const tag = tagOf(n);
		if (tag === '#text') {
			const raw = String(n['#text']).slice(skip).replace(/\s+/g, ' ');
			skip = 0;
			// Escape, then re-expose identifiers as code/links.
			const parts = raw.split(/(\b(?:shz|SHZ)_[A-Za-z0-9_]+(?:\(\))?)/);
			out += parts.map((p, k) => (k % 2 ? linkifyText(p) : esc(p))).join('');
			continue;
		}
		switch (tag) {
			case 'ref': {
				const label = plain(n);
				let suffix = '';
				const next = nodes[i + 1];
				if (isText(next) && String(next['#text']).startsWith('()')) {
					suffix = '()';
				}
				skip = suffix.length;
				const link = symbolLink(label, label + suffix);
				out += link || `\`${label}${suffix}\``;
				break;
			}
			case 'computeroutput': {
				const t = squash(plain(n));
				const m = t.match(/^((?:shz|SHZ)_[A-Za-z0-9_]+)(\(\))?$/);
				out += (m && symbolLink(m[1], t)) || '`' + t.replace(/`/g, "'") + '`';
				break;
			}
			case 'bold':
				out += `**${inline(kids(n), state).trim()}**`;
				break;
			case 'emphasis':
				out += `*${inline(kids(n), state).trim()}*`;
				break;
			case 'ulink':
				out += `[${inline(kids(n), state).trim()}](${attr(n, 'url')})`;
				break;
			case 'linebreak':
				out += ' ';
				break;
			case 'sp':
				out += ' ';
				break;
			case 'superscript':
				out += '^' + plain(n);
				break;
			case 'subscript':
				out += '_' + plain(n);
				break;
			case 'formula':
				out += '`' + squash(plain(n)).replace(/^\$|\$$/g, '') + '`';
				break;
			case 'anchor':
			case 'image':
			case 'htmlonly':
			case 'latexonly':
			case 'rtfonly':
			case 'manonly':
			case 'xmlonly':
			case 'docbookonly':
			case 'indexentry':
				break;
			default:
				if (tag in ENTITIES) out += ENTITIES[tag];
				else if (kids(n).length) out += inline(kids(n), state);
		}
	}
	return out;
}

const BLOCK_TAGS = new Set([
	'itemizedlist', 'orderedlist', 'programlisting', 'verbatim', 'preformatted', 'simplesect', 'parameterlist',
	'xrefsect', 'table', 'blockquote', 'heading', 'hruler', 'sect1', 'sect2', 'sect3', 'variablelist', 'title',
]);

// Converts a description node into { md, returns, notes, warnings, sa, params, deprecated }.
function convertDescription(descNode) {
	const st = { returns: [], notes: [], warnings: [], sa: [], params: new Map(), deprecated: '', todo: [] };
	const md = blocks(kids(descNode), st).trim();
	return { md, ...st };
}

function blocks(nodes, st, indent = '') {
	const out = [];
	for (const n of nodes) {
		const tag = tagOf(n);
		if (tag === 'para') out.push(...paraBlocks(n, st, indent));
		else if (tag === 'sect1' || tag === 'sect2' || tag === 'sect3' || tag === 'internal') out.push(blocks(kids(n), st, indent));
		else if (tag === '#text' && squash(String(n['#text']))) out.push(indent + esc(squash(String(n['#text']))));
		else if (BLOCK_TAGS.has(tag)) out.push(...blockNode(n, st, indent));
	}
	return out.filter((b) => b && b.trim()).join('\n\n');
}

function paraBlocks(para, st, indent) {
	const out = [];
	let buf = [];
	const flush = () => {
		const t = inline(buf, st).replace(/\s+/g, ' ').trim();
		if (t) out.push(indent + t);
		buf = [];
	};
	for (const n of kids(para)) {
		if (BLOCK_TAGS.has(tagOf(n))) {
			flush();
			out.push(...blockNode(n, st, indent));
		} else buf.push(n);
	}
	flush();
	return out;
}

function listBlock(n, st, indent, ordered) {
	const items = children(n, 'listitem').map((li, k) => {
		const marker = ordered ? `${k + 1}. ` : '- ';
		const body = blocks(kids(li), st, indent + ' '.repeat(marker.length));
		return indent + marker + body.slice(indent.length + marker.length);
	});
	return items.join('\n');
}

function codeBlock(text, lang, indent) {
	const body = text.replace(/\s+$/, '').split('\n').map((l) => (l ? indent + l : l)).join('\n');
	return `${indent}\`\`\`${lang}\n${body}\n${indent}\`\`\``;
}

function blockNode(n, st, indent) {
	const tag = tagOf(n);
	switch (tag) {
		case 'itemizedlist':
			return [listBlock(n, st, indent, false)];
		case 'orderedlist':
			return [listBlock(n, st, indent, true)];
		case 'programlisting': {
			const lines = children(n, 'codeline').map((cl) => plain(cl));
			const min = Math.min(...lines.filter((l) => l.trim()).map((l) => l.match(/^ */)[0].length));
			const lang = (attr(n, 'filename') || '.c').replace(/^\./, '').replace(/^\{|\}$/g, '') || 'c';
			return [codeBlock(lines.map((l) => l.slice(min)).join('\n'), lang === 'cpp' || lang === 'cxx' ? 'cpp' : 'c', indent)];
		}
		case 'verbatim':
		case 'preformatted':
			return [codeBlock(plain(n), 'text', indent)];
		case 'simplesect': {
			const kind = attr(n, 'kind');
			const text = blocks(kids(n), st, '');
			if (kind === 'return') st.returns.push(text);
			else if (kind === 'note' || kind === 'remark' || kind === 'attention' || kind === 'pre' || kind === 'post' || kind === 'invariant')
				st.notes.push(kind === 'note' || kind === 'remark' ? text : `${kind[0].toUpperCase() + kind.slice(1)}: ${text}`);
			else if (kind === 'warning') st.warnings.push(text);
			else if (kind === 'see') {
				for (const r of collect(n, 'ref')) st.sa.push(squash(plain(r)).replace(/\(\)$/, ''));
				for (const m of plain(n).matchAll(/\b(?:shz|SHZ)_[A-Za-z0-9_]+/g)) st.sa.push(m[0]);
			} else if (kind === 'par') {
				const title = child(n, 'title');
				return [`${indent}**${squash(plain(title || []))}** ${blocks(kids(n).filter((c) => tagOf(c) !== 'title'), st, '')}`];
			}
			return []; // author, copyright, version, date, since...
		}
		case 'parameterlist': {
			const kind = attr(n, 'kind');
			for (const item of children(n, 'parameteritem')) {
				const names = collect(item, 'parametername').map((p) => squash(plain(p)));
				const desc = blocks(kids(child(item, 'parameterdescription')), st, '').replace(/\n+/g, ' ');
				if (kind === 'retval') st.returns.push(`\`${names.join(', ')}\`: ${desc}`);
				else for (const nm of names) st.params.set(nm, desc);
			}
			return [];
		}
		case 'xrefsect': {
			const title = squash(plain(child(n, 'xreftitle') || []));
			const desc = blocks(kids(child(n, 'xrefdescription')), st, '');
			if (/deprecated/i.test(title)) st.deprecated = desc || 'Deprecated.';
			else if (/todo/i.test(title)) st.todo.push(desc);
			return [];
		}
		case 'table': {
			const rows = children(n, 'row').map((r) =>
				children(r, 'entry').map((e) => blocks(kids(e), st, '').replace(/\n+/g, ' ').replace(/\|/g, '\\|')),
			);
			if (!rows.length) return [];
			const w = Math.max(...rows.map((r) => r.length));
			const line = (r) => `${indent}| ${Array.from({ length: w }, (_, k) => r[k] || '').join(' | ')} |`;
			return [[line(rows[0]), `${indent}|${' --- |'.repeat(w)}`, ...rows.slice(1).map(line)].join('\n')];
		}
		case 'blockquote':
			return [blocks(kids(n), st, '').split('\n').map((l) => `${indent}> ${l}`).join('\n')];
		case 'heading':
			return [`${indent}#### ${squash(inline(kids(n), st))}`];
		case 'variablelist':
			return [];
		default:
			return [blocks(kids(n), st, indent)];
	}
}

function collect(n, tag, out = []) {
	for (const c of kids(n)) {
		if (tagOf(c) === tag) out.push(c);
		else if (!isText(c)) collect(c, tag, out);
	}
	return out;
}

// ---------------------------------------------------------------------------
// Collect C symbols

const moduleByFile = new Map(MODULES.map((m) => [m.file, m]));
// shz_sh4zam.h only includes the others.
for (const f of fs.readdirSync(INC).filter((f) => f.endsWith('.h') && f !== 'shz_sh4zam.h' && !moduleByFile.has(f)).sort())
	console.warn(`gen-api: ${f} has no entry in MODULES (scripts/data/modules.mjs); its symbols are skipped`);
const symbols = [];
const seen = new Set();
const sectionOrder = new Map(); // module -> [section names in header order]

function sectionTitle(sec) {
	const header = child(sec, 'header');
	if (header) return squash(plain(header));
	return { func: 'Functions', define: 'Macros', typedef: 'Types', enum: 'Enumerations', var: 'Variables' }[attr(sec, 'kind')] || 'Other';
}

function addSection(mod, title) {
	if (!sectionOrder.has(mod)) sectionOrder.set(mod, []);
	const list = sectionOrder.get(mod);
	if (!list.includes(title)) list.push(title);
}

function legacyUrl(id) {
	const k = id.lastIndexOf('_1');
	return k < 0 ? null : { page: `${id.slice(0, k)}.html`, anchor: id.slice(k + 2) };
}

// SHZ_TAREGT was a typo in shz_cdefs.h's fallback branch: never document it.
const SKIP_NAME = /(_sh4|_sw|_spu)$|^SHZ_\w+_H(PP)?$|^SHZ_TAREGT$/;

// Doc text for a macro straight from the header: `//!<` trailing or `//!` preceding
// comments on any of its #define lines, else a plain `//` comment next to it.
function sourceComment(file, name, line) {
	const lines = srcLines(file);
	const defs = [];
	lines.forEach((l, i) => {
		if (new RegExp(`^\\s*#\\s*define\\s+${name}\\b`).test(l)) defs.push(i);
	});
	if (!defs.length && line) defs.push(line - 1);
	const tidy = (t) =>
		linkifyText(
			esc(squash(t.replace(/[\\@](?:p|a|c)\s+(\S+)/g, '\u0001$1\u0001').replace(/[\\@]b\s+(\S+)/g, '$1')))
				.replace(/\\`/g, '`')
				.replace(/\u0001([^\u0001]*)\u0001/g, '`$1`'),
		);
	const upward = (i, re) => {
		const doc = [];
		let k = i - 1;
		while (k >= 0 && (re.test(lines[k]) || (doc.length === 0 && /^\s*#\s*(if|ifdef|ifndef|elif|else)\b/.test(lines[k])))) {
			if (re.test(lines[k])) doc.unshift(lines[k].replace(/^\s*\/\/!?\s?/, ''));
			k--;
		}
		return doc.join(' ');
	};
	for (const i of defs) {
		const t = (lines[i].match(/\/\/!<\s*(.*)$/) || [])[1] || upward(i, /^\s*\/\/!/);
		if (t) return { text: tidy(t), doc: true };
	}
	for (const i of defs) {
		const t = upward(i, /^\s*\/\/(?!!)/) || (lines[i].match(/\/\/\s*(.*)$/) || [])[1];
		if (t) return { text: tidy(t), doc: false };
	}
	return { text: '', doc: false };
}
const sectionDocs = new Map(); // `${module}/${section}` -> description md

function memberRecord(md, mod, section, compoundId) {
	const name = squash(plain(child(md, 'name')));
	const kind = attr(md, 'kind');
	if (!/^(shz_|SHZ_)/.test(name) || SKIP_NAME.test(name)) return null;
	const loc = child(md, 'location');
	const file = attr(loc, 'file');
	const line = Number(attr(loc, 'line'));
	renderCtx = name;
	const brief = convertDescription(child(md, 'briefdescription'));
	const detail = convertDescription(child(md, 'detaileddescription'));
	const params = children(md, 'param').map((p) => {
		const pname = squash(plain(child(p, 'declname') || child(p, 'defname') || []));
		const ptype = squash(plain(child(p, 'type') || []) + ' ' + plain(child(p, 'array') || []));
		return { name: pname, type: ptype, doc: detail.params.get(pname) || brief.params.get(pname) || '' };
	});
	let signature;
	let kindOut;
	if (kind === 'function') {
		kindOut = 'function';
		signature = rawDeclaration(file, line, name) + ';';
	} else if (kind === 'define') {
		kindOut = 'macro';
		const line0 = srcLines(file)[line - 1] || '';
		const m = line0.match(new RegExp(`#\\s*define\\s+${name}(\\([^)]*\\))?\\s*(.*)$`));
		const argsTxt = m?.[1] || '';
		let value = (m?.[2] || '').replace(/\/\/.*$/, '').replace(/\/\*.*?\*\//g, '').trim();
		if (value.endsWith('\\') || value.length > 90) value = '';
		signature = `#define ${name}${argsTxt}${value ? ' ' + value : ''}`;
		if (!params.length && argsTxt) {
			for (const a of argsTxt.slice(1, -1).split(',').map((s) => s.trim()).filter(Boolean)) params.push({ name: a, type: '', doc: detail.params.get(a) || '' });
		}
	} else if (kind === 'typedef') {
		kindOut = 'typedef';
		signature = rawDeclaration(file, line, name) + ';';
	} else if (kind === 'enum') {
		kindOut = 'enum';
		const end = Number(attr(loc, 'bodyend')) > 0 ? Number(attr(loc, 'bodyend')) : line;
		signature = rawBlock(file, Number(attr(loc, 'bodystart')) || line, end);
	} else if (kind === 'variable') {
		kindOut = 'variable';
		signature = rawDeclaration(file, line, name) + ';';
	} else return null;
	const retType = kind === 'function' ? squash(plain(child(md, 'type') || [])) : '';
	const legacy = legacyUrl(attr(md, 'id'));
	const rec = {
		name,
		kind: kindOut,
		module: mod.id,
		section,
		header: `sh4zam/${baseName(file)}`,
		line,
		signature,
		return_type: retType || undefined,
		params: kind === 'function' || kind === 'define' ? params : undefined,
		brief: brief.md.replace(/\n+/g, ' '),
		detail: detail.md,
		returns: [...brief.returns, ...detail.returns].join(' '),
		notes: [...brief.notes, ...detail.notes],
		warnings: [...brief.warnings, ...detail.warnings],
		sa: [...new Set([...brief.sa, ...detail.sa])].filter((s) => s !== name),
		deprecated: detail.deprecated || brief.deprecated || '',
		enumerators: kind === 'enum' ? children(md, 'enumvalue').map((ev) => ({ name: squash(plain(child(ev, 'name'))), brief: convertDescription(child(ev, 'briefdescription')).md })) : undefined,
		_xmlid: attr(md, 'id'),
		_legacy: legacy,
		_compound: compoundId,
	};
	rec.documented = Boolean(rec.brief || rec.detail);
	if (!rec.documented && kind === 'define') {
		const sc = sourceComment(file, name, line);
		rec.brief = sc.text;
		rec.documented = sc.doc;
	}
	return rec;
}

for (const mod of MODULES) {
	const id = mod.file.replace(/_/g, '__').replace('.h', '_8h');
	const cd = compounddef(id);
	if (!cd) throw new Error(`missing Doxygen XML for ${mod.file}`);
	for (const sec of children(cd, 'sectiondef')) {
		const title = sectionTitle(sec);
		const desc = child(sec, 'description');
		if (desc) {
			renderCtx = `${mod.id} section ${title}`;
			const d = convertDescription(desc).md;
			if (d) sectionDocs.set(`${mod.id}/${title}`, d);
		}
		for (const md of children(sec, 'memberdef')) {
			const rec = memberRecord(md, mod, title, id);
			if (!rec || seen.has(rec.name)) continue;
			seen.add(rec.name);
			addSection(mod.id, title);
			symbols.push(rec);
		}
	}
	// Structs defined in this header.
	for (const ic of children(cd, 'innerclass')) {
		const sid = attr(ic, 'refid');
		const sd = compounddef(sid);
		const name = squash(plain(child(sd, 'compoundname')));
		if (!/^shz_\w+$/.test(name) || seen.has(name)) continue;
		const loc = child(sd, 'location');
		const file = attr(loc, 'file');
		const start = Number(attr(loc, 'bodystart')) || Number(attr(loc, 'line'));
		const end = Number(attr(loc, 'bodyend')) || start;
		// Include the typedef line (and anything up to the struct's closing name).
		let defStart = start;
		const lines = srcLines(file);
		while (defStart > 1 && !/typedef|struct/.test(lines[defStart - 1])) defStart--;
		renderCtx = name;
		const brief = convertDescription(child(sd, 'briefdescription'));
		const detail = convertDescription(child(sd, 'detaileddescription'));
		const fields = [];
		const walk = (cid) => {
			const c = compounddef(cid);
			if (!c) return;
			for (const sec of children(c, 'sectiondef'))
				for (const md of children(sec, 'memberdef')) {
					const fname = squash(plain(child(md, 'name')));
					if (fname.startsWith('@')) continue;
					fields.push({
						name: fname,
						type: squash(plain(child(md, 'type') || []) + plain(child(md, 'argsstring') || [])),
						doc: convertDescription(child(md, 'briefdescription')).md,
					});
				}
			for (const inner of children(c, 'innerclass')) walk(attr(inner, 'refid'));
		};
		walk(sid);
		seen.add(name);
		addSection(mod.id, 'Types');
		symbols.push({
			name,
			kind: 'type',
			module: mod.id,
			section: 'Types',
			header: `sh4zam/${baseName(file)}`,
			line: Number(attr(loc, 'line')),
			signature: rawBlock(file, defStart, end),
			brief: brief.md.replace(/\n+/g, ' '),
			detail: detail.md,
			returns: '',
			notes: [...brief.notes, ...detail.notes],
			warnings: [...brief.warnings, ...detail.warnings],
			sa: [...new Set([...brief.sa, ...detail.sa])].filter((s) => s !== name),
			deprecated: '',
			fields,
			documented: Boolean(brief.md || detail.md),
			_xmlid: sid,
			_legacy: { page: `${sid}.html`, anchor: '' },
			_compound: sid,
		});
	}
}

// Enumerators become index entries pointing at their enum's page.
for (const s of [...symbols]) {
	for (const ev of s.enumerators || []) {
		if (seen.has(ev.name)) continue;
		seen.add(ev.name);
		symbols.push({ name: ev.name, kind: 'enumerator', module: s.module, section: s.section, header: s.header, line: s.line, signature: ev.name, brief: ev.brief || `Enumerator of \`${s.name}\`.`, detail: '', returns: '', notes: [], warnings: [], sa: [s.name], deprecated: '', documented: Boolean(ev.brief), _parent: s.name });
	}
}

// Paths (needed by the link resolver before any rendering). Page slugs are the exact C
// identifier, except when two names in a module differ only by case (SHZ_VERSION vs the
// shz_version typedef): case-insensitive filesystems can't hold both, so the later one
// gets a `-<kind>` suffix.
const usedSlugs = new Set();
function pageSlug(s) {
	if (s.kind === 'enumerator') return s.name;
	let slug = s.name;
	if (usedSlugs.has(`${s.module}/${slug.toLowerCase()}`)) slug = `${s.name}-${s.kind}`;
	usedSlugs.add(`${s.module}/${slug.toLowerCase()}`);
	return slug;
}
for (const s of symbols) {
	s.slug = pageSlug(s);
	s.path = s.kind === 'enumerator' ? `/api/${s.module}/${symbolsByName.get(s._parent)?.slug || s._parent}/#${s.name.toLowerCase()}` : `/api/${s.module}/${s.slug}/`;
	symbolsByName.set(s.name, s);
}

// Re-render descriptions now that every symbol can be linked.
unresolved.clear();
{
	const again = new Map();
	for (const mod of MODULES) {
		const id = mod.file.replace(/_/g, '__').replace('.h', '_8h');
		const cd = compounddef(id);
		for (const sec of children(cd, 'sectiondef')) {
			const title = sectionTitle(sec);
			const desc = child(sec, 'description');
			if (desc) {
				renderCtx = `${mod.id} section ${title}`;
				const d = convertDescription(desc).md;
				if (d) sectionDocs.set(`${mod.id}/${title}`, d);
			}
			for (const md of children(sec, 'memberdef')) {
				const name = squash(plain(child(md, 'name')));
				if (!symbolsByName.has(name) || again.has(name)) continue;
				again.set(name, memberRecord(md, mod, title, id));
			}
		}
	}
	for (const s of symbols) {
		const r = again.get(s.name);
		if (!r) continue;
		for (const k of ['brief', 'detail', 'returns', 'notes', 'warnings', 'params', 'deprecated', 'enumerators', 'documented']) s[k] = r[k];
	}
	// Struct descriptions and field docs.
	for (const s of symbols.filter((x) => x.kind === 'type')) {
		const sd = compounddef(s._xmlid);
		renderCtx = s.name;
		s.brief = convertDescription(child(sd, 'briefdescription')).md.replace(/\n+/g, ' ');
		const d = convertDescription(child(sd, 'detaileddescription'));
		s.detail = d.md;
	}
}

// Symbols Doxygen did not see at all (e.g. hidden in an unnamed \cond block): scan headers.
const doxyNames = new Set(symbols.map((s) => s.name));
const headerScan = [];
for (const mod of MODULES) {
	const lines = srcLines(path.join(INC, mod.file));
	let inCpp = false;
	lines.forEach((l, i) => {
		if (/^#\s*ifdef\s+__cplusplus|^#\s*else\s*\/\/\s*C\+\+/.test(l)) inCpp = true;
		const fm = l.match(/^\s*(?:SHZ_\w+\s+)*(?:const\s+)?[\w]+\s*\*?\s+\*?(shz_[a-z0-9_]+)\s*\(/);
		if (fm && !doxyNames.has(fm[1]) && !SKIP_NAME.test(fm[1]) && !inCpp) headerScan.push({ name: fm[1], mod, line: i + 1, kind: 'function' });
		const dm = l.match(/^\s*#\s*define\s+((?:SHZ|shz)_[A-Za-z0-9_]+)/);
		if (dm && !doxyNames.has(dm[1]) && !SKIP_NAME.test(dm[1])) headerScan.push({ name: dm[1], mod, line: i + 1, kind: 'macro' });
	});
}
const PRIVATE_MACRO = /^SHZ_(THREAD_LOCAL|STRINGIFY_LITERAL)$|^SHZ_\w+_H$/;
for (const h of headerScan) {
	if (seen.has(h.name) || PRIVATE_MACRO.test(h.name)) continue;
	seen.add(h.name);
	const file = path.join(INC, h.mod.file);
	const lines = srcLines(file);
	renderCtx = h.name;
	const sc = sourceComment(file, h.name, h.line);
	const brief = sc.text;
	let signature;
	if (h.kind === 'macro') {
		const m = lines[h.line - 1].match(new RegExp(`#\\s*define\\s+${h.name}(\\([^)]*\\))?\\s*(.*)$`));
		let value = (m?.[2] || '').replace(/\/\/.*$/, '').trim();
		if (value.endsWith('\\') || value.length > 90) value = '';
		signature = `#define ${h.name}${m?.[1] || ''}${value ? ' ' + value : ''}`;
	} else signature = rawDeclaration(file, h.line, h.name) + ';';
	addSection(h.mod.id, 'Not in upstream reference');
	const sym = {
		name: h.name, kind: h.kind, module: h.mod.id, section: 'Not in upstream reference', header: `sh4zam/${h.mod.file}`, line: h.line,
		signature, brief, detail: '', returns: '', notes: [], warnings: [], sa: [], deprecated: '', documented: sc.doc, _scanned: true,
	};
	sym.slug = pageSlug(sym);
	sym.path = `/api/${sym.module}/${sym.slug}/`;
	symbols.push(sym);
	symbolsByName.set(sym.name, sym);
}

// ---------------------------------------------------------------------------
// C++ API

const cppClasses = []; // { name, title, module, ctype, brief, detail, members: [{name, sigs[], brief, calls[], resolved[], static}] }
const cppAliases = []; // { name: 'shz::sinf', target: 'shz_sinf', brief, module }
const cppConstants = []; // { name: 'shz::pi_f', decl, brief }

function resolveCall(call, ctype) {
	if (genericTable.has(call)) {
		if (ctype) return genericTable.get(call).get(ctype) || call;
		return call;
	}
	return call;
}

function cppMember(md) {
	const name = squash(plain(child(md, 'name')));
	const loc = child(md, 'location');
	const file = attr(loc, 'file');
	const line = Number(attr(loc, 'line'));
	const kind = attr(md, 'kind');
	const def = squash(plain(child(md, 'definition') || []));
	const argsstring = squash(plain(child(md, 'argsstring') || []));
	const body = kind === 'function' ? bodyAfter(file, line) : '';
	// Doxygen drops `= shz_foo` initializers of `constexpr auto` aliases; read the source line.
	const init = squash(plain(child(md, 'initializer') || [])) + (kind === 'function' ? '' : ' ' + rawDeclaration(file, line, name).replace(/^[^=]*/, ''));
	const calls = [...new Set([...(body + ' ' + init).matchAll(/\b(shz_[a-z0-9_]+)\s*[(;,]?/g)].map((m) => m[1]).filter((c) => !/_t$/.test(c)))];
	renderCtx = `C++ ${def}`;
	const brief = convertDescription(child(md, 'briefdescription')).md.replace(/\n+/g, ' ');
	return { name, kind, def, argsstring, brief, calls, static: attr(md, 'static') === 'yes', file: baseName(file), line };
}

{
	const ns = compounddef('namespaceshz');
	for (const sec of children(ns, 'sectiondef'))
		for (const md of children(sec, 'memberdef')) {
			const m = cppMember(md);
			const target = m.calls.find((c) => symbolsByName.has(c));
			if (!target) {
				const decl = rawDeclaration(path.join(INC, m.file), m.line, m.name);
				if (m.kind === 'variable' && /constexpr/.test(decl)) cppConstants.push({ name: `shz::${m.name}`, decl: decl + ';', brief: m.brief });
				continue;
			}
			cppAliases.push({ name: `shz::${m.name}`, target, brief: m.brief, kind: m.kind, def: m.def, file: m.file, line: m.line, module: symbolsByName.get(target).module });
		}
	for (const ic of children(ns, 'innerclass')) {
		const id = attr(ic, 'refid');
		const cd = compounddef(id);
		const full = squash(plain(child(cd, 'compoundname')));
		const short = full.replace(/^shz::/, '');
		const meta = CPP_CLASSES[short];
		if (!meta) {
			console.warn(`gen-api: C++ class ${full} has no entry in CPP_CLASSES; skipped`);
			continue;
		}
		renderCtx = full;
		const loc = child(cd, 'location');
		const cls = {
			name: short,
			slug: short.toLowerCase(),
			title: meta.title,
			module: meta.module,
			ctype: meta.ctype,
			header: `sh4zam/${baseName(attr(loc, 'file'))}`,
			line: Number(attr(loc, 'line')),
			brief: convertDescription(child(cd, 'briefdescription')).md.replace(/\n+/g, ' '),
			detail: convertDescription(child(cd, 'detaileddescription')).md,
			bases: children(cd, 'basecompoundref').map((b) => squash(plain(b))),
			members: new Map(),
		};
		const add = (md, inherited) => {
			const m = cppMember(md);
			if (/^(CppType|CType|Rows|Cols)$/.test(m.name)) return;
			if (!cls.members.has(m.name)) cls.members.set(m.name, { name: m.name, sigs: [], brief: m.brief, calls: new Set(), static: m.static, inherited, line: m.line, file: m.file });
			const e = cls.members.get(m.name);
			const sig = squash(`${m.def}${m.argsstring}`).replace(/\bshz::(\w+)::/g, '');
			if (!e.sigs.includes(sig)) e.sigs.push(sig);
			if (!e.brief && m.brief) e.brief = m.brief;
			for (const c of m.calls) e.calls.add(resolveCall(c, meta.ctype));
		};
		for (const sec of children(cd, 'sectiondef')) for (const md of children(sec, 'memberdef')) add(md, false);
		// Inherited members (vecN base) from listofallmembers.
		const lam = child(cd, 'listofallmembers');
		for (const mem of children(lam, 'member')) {
			const refid = attr(mem, 'refid');
			const owner = refid.slice(0, refid.lastIndexOf('_1'));
			if (owner === id) continue;
			const ocd = compounddef(owner);
			const omd = ocd && collect(ocd, 'memberdef').find((x) => attr(x, 'id') === refid);
			if (omd) add(omd, true);
		}
		cppClasses.push(cls);
	}
	cppClasses.sort((a, b) => Object.keys(CPP_CLASSES).indexOf(a.name) - Object.keys(CPP_CLASSES).indexOf(b.name));
}

// C -> C++ mapping: aliases, and class members whose body calls exactly one C function.
const cppOf = new Map();
const addCpp = (c, cpp) => {
	if (!symbolsByName.has(c)) return;
	if (!cppOf.has(c)) cppOf.set(c, new Set());
	cppOf.get(c).add(cpp);
};
for (const a of cppAliases) addCpp(a.target, a.name);
for (const cls of cppClasses) if (cls.ctype) addCpp(cls.ctype, `shz::${cls.name}`);
const unresolvedCpp = [];
for (const cls of cppClasses) {
	for (const m of cls.members.values()) {
		const known = [...m.calls].filter((c) => symbolsByName.has(c));
		m.resolved = known;
		if (known.length === 1 && cls.name !== 'vecN') addCpp(known[0], `shz::${cls.name}::${m.name}`);
		else if (!known.length && m.calls.size) unresolvedCpp.push(`shz::${cls.name}::${m.name} -> ${[...m.calls].join(', ')}`);
	}
}

for (const s of symbols) {
	s.cpp = [...(cppOf.get(s.name) || [])].sort();
	const g = genericOf.get(s.name);
	s.generic = g && symbolsByName.has(g) ? g : null;
	if (s.kind === 'function') {
		s.backends = { sh4: SH4_IMPL.has(s.name) ? 'asm' : 'c', sw: 'c', spu: SPU_IMPL.has(s.name) ? 'simd' : 'c' };
	}
	renderCtx = `${s.name} (see also)`;
	s.sa = s.sa.filter((x, i, a) => a.indexOf(x) === i);
	for (const x of s.sa) if (!symbolsByName.has(x)) noteUnresolved(x);
}

// Order: module order, section order (header order), then line.
const modIndex = new Map(MODULES.map((m, i) => [m.id, i]));
symbols.sort((a, b) => {
	const d = modIndex.get(a.module) - modIndex.get(b.module);
	if (d) return d;
	const so = sectionOrder.get(a.module);
	return so.indexOf(a.section) - so.indexOf(b.section) || a.line - b.line || a.name.localeCompare(b.name);
});

// ---------------------------------------------------------------------------
// Rendering helpers

const files = new Map(); // relative path -> content
const emit = (rel, content) => files.set(rel.replace(/\\/g, '/'), content.endsWith('\n') ? content : content + '\n');
const yamlStr = (s) => JSON.stringify(s);
const modById = new Map(MODULES.map((m) => [m.id, m]));
const tableCell = (s) => (s || '').replace(/\|/g, '\\|').replace(/\n+/g, ' ');

function stripMd(s) {
	return s
		.replace(/\[([^\]]*)\]\([^)]*\)/g, '$1')
		.replace(/[`*]/g, '')
		.replace(/\\([\\*\[\]<`|])/g, '$1')
		.replace(/\s+/g, ' ')
		.trim();
}

function description(s, fallback) {
	let d = stripMd(s || '') || fallback;
	if (d.length > 160) {
		const cut = d.slice(0, 157);
		d = cut.slice(0, cut.lastIndexOf(' ')).replace(/[,;:.]$/, '') + '...';
	}
	return d.replace(/\.$/, '');
}

function frontmatter(o) {
	const lines = ['---', `title: ${yamlStr(o.title)}`, `description: ${yamlStr(o.description)}`];
	// Exact-case URLs: Astro would otherwise lowercase SHZ_* macro slugs.
	if (o.slug) lines.push(`slug: ${yamlStr(o.slug)}`);
	if (o.hidden) lines.push('sidebar:', '  hidden: true');
	if (o.label) lines.push('sidebar:', `  label: ${yamlStr(o.label)}`);
	lines.push('editUrl: false');
	if (o.toc === false) lines.push('tableOfContents: false');
	else if (o.toc) lines.push('tableOfContents:', `  minHeadingLevel: ${o.toc[0]}`, `  maxHeadingLevel: ${o.toc[1]}`);
	if (o.pagefind === false) lines.push('pagefind: false');
	lines.push('---', '');
	return lines.join('\n');
}

const GENERATED_NOTE = (what) => `<!-- Generated by scripts/gen-api.mjs from ${what} @ ${COMMIT.slice(0, 7)}. Do not edit. -->\n`;

function proto(code, lang = 'c') {
	return `<div class="shz-proto">\n\n\`\`\`${lang}\n${code}\n\`\`\`\n\n</div>\n`;
}

function cppLink(name) {
	// shz::vec3::cross -> /api/cpp/vec3/#cross ; shz::sinf -> /api/cpp/#shzsinf
	const parts = name.split('::');
	const cls = cppClasses.find((c) => `shz::${c.name}` === name);
	if (cls) return `[\`${name}\`](/api/cpp/${cls.slug}/)`;
	if (parts.length === 3) {
		const anchor = /^[a-z0-9_]+$/i.test(parts[2]) ? `#${parts[2].toLowerCase()}` : '';
		return `[\`${name}\`](/api/cpp/${parts[1].toLowerCase()}/${anchor})`;
	}
	return `[\`${name}\`](/api/cpp/#${name.replace(/::/g, '').toLowerCase()})`;
}

const KIND_LABEL = { function: 'Function', macro: 'Macro', type: 'Type', typedef: 'Typedef', enum: 'Enum', enumerator: 'Enumerator', variable: 'Variable' };

function backendLine(b) {
	if (!b) return 'n/a (not a function)';
	const sh4 = b.sh4 === 'asm' ? 'SH4: dedicated SH4 implementation' : 'SH4: shared C implementation';
	const spu = b.spu === 'simd' ? 'SPU: dedicated SPU implementation' : 'SPU: shared C implementation';
	return `${sh4} · SW: portable C · ${spu}`;
}

// Library-wide rules that apply to a symbol because of its name or parameter types,
// so readers don't need the suffix page to use it safely.
const SUFFIX_RULES = [
	[/_fsrra$/, '_fsrra'],
	[/_safe$/, '_safe'],
	[/_deg$/, '_deg'],
	[/u16$/, 'u16'],
	[/_unaligned(_|$)/, '_unaligned'],
	[/_transpose(_|$)/, '_transpose'],
	[/_reverse(_|$)/, '_reverse'],
	[/_(wxyz|yzwx|wzyx)(_|$)/, '_wxyz'],
	[/_xmtrx$/, '_xmtrx'],
	[/^shz_sq_/, 'sq_'],
	[/_dot[23]$/, 'dot2/dot3'],
];
function namingNotes(s) {
	if (s.kind !== 'function' && s.kind !== 'macro') return [];
	const notes = [];
	for (const [re, key] of SUFFIX_RULES) if (re.test(s.name) && !(key === '_safe' && s.deprecated)) notes.push(`\`${key}\`: ${SUFFIXES[key]} ([Naming and Suffixes](/concepts/naming-and-suffixes/))`);
	const aligned = (s.params || []).some((p) => /shz_mat(4x4|2x2)_t\s*\*/.test(p.type));
	if (aligned) notes.push('`shz_mat4x4_t` and `shz_mat2x2_t` arguments must be 8-byte aligned. Unaligned data goes through the `float[16]` parameters of the `_unaligned` routines.');
	return notes;
}

// ---------------------------------------------------------------------------
// Symbol pages

for (const s of symbols) {
	if (s.kind === 'enumerator') continue;
	const mod = modById.get(s.module);
	const out = [];
	out.push(frontmatter({ title: s.name, slug: `api/${s.module}/${s.slug}`, description: description(s.brief, `${KIND_LABEL[s.kind]} ${s.name} in the SH4ZAM ${mod.title} API`), hidden: true, toc: false }));
	out.push(GENERATED_NOTE(`${s.header}:${s.line}`));
	out.push(proto(s.signature));
	out.push('<div class="shz-doc">\n');
	out.push(s.brief || '*No description in the upstream header.*');
	out.push('');
	if (s.deprecated) out.push(`**Deprecated:** ${s.deprecated}\n`);
	if (s.params && s.params.length) {
		out.push('## Parameters\n');
		out.push('| Name | Type | Description |', '| --- | --- | --- |');
		for (const p of s.params) out.push(`| \`${p.name}\` | ${p.type ? '`' + p.type + '`' : ''} | ${tableCell(p.doc)} |`);
		out.push('');
	}
	if (s.kind === 'function') {
		out.push('## Returns\n');
		out.push(s.returns || (s.return_type && s.return_type !== 'void' ? `\`${s.return_type}\`` : 'Nothing (`void`).'));
		out.push('');
	}
	if (s.fields && s.fields.length) {
		out.push('## Fields\n');
		out.push('| Field | Type | Description |', '| --- | --- | --- |');
		for (const f of s.fields) out.push(`| \`${f.name}\` | \`${f.type}\` | ${tableCell(f.doc)} |`);
		out.push('');
	}
	if (s.enumerators && s.enumerators.length) {
		out.push('## Values\n');
		for (const e of s.enumerators) out.push(`- <span id="${e.name.toLowerCase()}"></span>\`${e.name}\`${e.brief ? ': ' + e.brief.replace(/\n+/g, ' ') : ''}`);
		out.push('');
	}
	if (s.detail) out.push('## Details\n', s.detail, '');
	if (s.notes.length) out.push('## Notes\n', ...s.notes.map((n) => `- ${n.replace(/\n+/g, ' ')}`), '');
	if (s.warnings.length) out.push('## Warnings\n', ...s.warnings.map((n) => `- ${n.replace(/\n+/g, ' ')}`), '');
	const naming = namingNotes(s);
	if (naming.length) out.push('## Naming and requirements\n', ...naming.map((n) => `- ${n}`), '');
	out.push('</div>\n');
	out.push('## Availability\n');
	out.push(`- **C++ equivalent:** ${s.cpp.length ? s.cpp.map(cppLink).join(', ') : s.kind === 'function' ? 'none (call the C function from C++)' : 'none (usable from C++ as is)'}`);
	if (s.kind === 'function' || s.generic) out.push(`- **Type-generic form:** ${s.generic ? symbolLink(s.generic, s.generic + '()') : 'none'}`);
	out.push(`- **Back-ends:** ${backendLine(s.backends)}`);
	out.push(`- **Defined in:** [\`${s.header}:${s.line}\`](${srcUrl(s.header.replace('sh4zam/', ''), s.line)})`);
	out.push(`- **Module:** [${mod.title}](/api/${mod.id}/), section "${s.section}"`);
	out.push('');
	if (s.sa.length) {
		out.push('## See also\n');
		out.push(s.sa.map((x) => symbolLink(x, x) || `\`${x}\``).join(', '));
		out.push('');
	}
	emit(`src/content/docs/api/${s.module}/${s.slug}.md`, out.join('\n'));
}

// ---------------------------------------------------------------------------
// Module pages

for (const mod of MODULES) {
	const syms = symbols.filter((s) => s.module === mod.id && s.kind !== 'enumerator');
	const out = [];
	out.push(frontmatter({ title: `${mod.title} API`, description: description(mod.brief, mod.title), label: mod.title, toc: [2, 2] }));
	out.push(GENERATED_NOTE(`sh4zam/${mod.file}`));
	out.push(mod.intro.join(' '));
	out.push('');
	out.push(`- **C header:** \`#include <sh4zam/${mod.file}>\``);
	out.push(`- **C++ header:** \`#include <sh4zam/${mod.cpp_file}>\``);
	const classes = cppClasses.filter((c) => c.module === mod.id);
	if (classes.length) out.push(`- **C++ types:** ${classes.map((c) => `[\`${c.title}\`](/api/cpp/${c.slug}/)`).join(', ')}`);
	out.push(`- **Symbols:** ${syms.length}`);
	out.push('');
	if (mod.verbs) {
		out.push('## Verb model\n');
		out.push('For most transform types this API offers several versions of the same operation. Using translation as the example:\n');
		out.push('| Verb | Meaning |', '| --- | --- |');
		for (const [v, d] of VERBS) out.push(`| \`${v}\` | ${d} |`);
		out.push('');
	}
	if (mod.suffixes.length) {
		out.push('## Suffixes\n');
		out.push('Suffixes used in this module. The full list is in [Naming and Suffixes](/concepts/naming-and-suffixes/).\n');
		for (const k of mod.suffixes) out.push(`- \`${k}\`: ${SUFFIXES[k]}`);
		out.push('');
	}
	for (const sec of sectionOrder.get(mod.id) || []) {
		const rows = syms.filter((s) => s.section === sec);
		if (!rows.length) continue;
		out.push(`## ${sec}\n`);
		const d = sectionDocs.get(`${mod.id}/${sec}`);
		if (d) out.push(d.replace(/\n+/g, ' '), '');
		if (sec === 'Not in upstream reference')
			out.push('Public declarations that upstream keeps out of its Doxygen reference. Listed for completeness; prefer the documented API.\n');
		out.push('| Symbol | Description |', '| --- | --- |');
		for (const s of rows) out.push(`| [\`${s.name}\`](${s.path}) | ${tableCell(s.brief) || '*Undocumented upstream.*'} |`);
		out.push('');
	}
	emit(`src/content/docs/api/${mod.id}/index.md`, out.join('\n'));
}

// ---------------------------------------------------------------------------
// API index page

{
	const out = [];
	out.push(frontmatter({ title: 'API Reference', description: 'Every public SH4ZAM symbol, one page each, grouped by module', label: 'Overview', toc: false }));
	out.push(GENERATED_NOTE('the public headers'));
	out.push(
		`SH4ZAM ${VERSION} exposes ${symbols.filter((s) => s.kind !== 'enumerator').length} public C symbols across ${MODULES.length} modules, plus a C++ API in \`namespace shz\`. Every symbol has its own page at \`/api/<module>/<name>/\` with a Markdown twin at \`/api/<module>/<name>.md\`.`,
	);
	out.push('');
	out.push('| Module | Header | Symbols | Description |', '| --- | --- | --- | --- |');
	for (const mod of MODULES) {
		const n = symbols.filter((s) => s.module === mod.id && s.kind !== 'enumerator').length;
		out.push(`| [${mod.title}](/api/${mod.id}/) | \`sh4zam/${mod.file}\` | ${n} | ${tableCell(mod.brief)} |`);
	}
	out.push('');
	out.push('## Other ways in\n');
	out.push('- [C++ API](/api/cpp/): `shz::vec3`, `shz::quat`, `shz::xmtrx` and the free-function aliases, mapped back to the C functions.');
	out.push('- [Cheatsheet](/cheatsheet/): the whole API on one page, signature plus one line each.');
	out.push('- [`/api/index.json`](/api/index.json): machine-readable symbol index ([schema](/api/schema.json)). Per-module files live at `/api/<module>.json`.');
	out.push('- Search (top of every page) indexes symbol names and descriptions.');
	out.push('');
	out.push('## Naming in one line\n');
	out.push('C functions are `shz_<module>_<verb>[_<qualifier>]`, types are `shz_<name>_t`, C++ lives in `namespace shz` with the `shz_` prefix dropped. Suffixes such as `_fsrra`, `_safe`, `_deg` and `_unaligned` change behavior; see [Naming and Suffixes](/concepts/naming-and-suffixes/).');
	emit('src/content/docs/api/index.md', out.join('\n'));
}

// ---------------------------------------------------------------------------
// C++ pages

{
	const out = [];
	out.push(frontmatter({ title: 'C++ API', description: 'The SH4ZAM C++ API in namespace shz: classes, operators and aliases, mapped to the C functions they call', label: 'Overview', toc: [2, 2] }));
	out.push(GENERATED_NOTE('the .hpp headers'));
	out.push('C++ code can still use the C API by design, and every C++ type is also compatible with its corresponding C types and C API, so you can mix and match.');
	out.push('');
	out.push('Include `<sh4zam/shz_sh4zam.hpp>` (or a per-module `.hpp`). Vector, quaternion and matrix types derive from the C structs and add methods and operators; `shz::xmtrx` exposes the active matrix as static member functions; scalar and trig routines are `constexpr` aliases of the C functions with the `shz_` prefix dropped.');
	out.push('');
	out.push('## Classes\n');
	out.push('| Class | C type | Header | Description |', '| --- | --- | --- | --- |');
	for (const c of cppClasses) out.push(`| [\`${c.title}\`](/api/cpp/${c.slug}/) | ${c.ctype ? symbolLink(c.ctype) || '`' + c.ctype + '`' : 'n/a'} | \`${c.header}\` | ${tableCell(c.brief)} |`);
	out.push('');
	const byMod = new Map();
	for (const a of cppAliases) {
		if (!byMod.has(a.module)) byMod.set(a.module, []);
		byMod.get(a.module).push(a);
	}
	for (const mod of MODULES) {
		const list = byMod.get(mod.id);
		if (!list) continue;
		out.push(`## ${mod.title} aliases\n`);
		out.push('| C++ | C function | Description |', '| --- | --- | --- |');
		for (const a of list) out.push(`| <span id="${a.name.replace(/::/g, '').toLowerCase()}"></span>\`${a.name}\` | ${symbolLink(a.target)} | ${tableCell(symbolsByName.get(a.target).brief)} |`);
		out.push('');
	}
	if (cppConstants.length) {
		out.push('## Constants\n');
		out.push('| C++ | Definition | Description |', '| --- | --- | --- |');
		for (const c of cppConstants) out.push(`| <span id="${c.name.replace(/::/g, '').toLowerCase()}"></span>\`${c.name}\` | \`${c.decl}\` | ${tableCell(c.brief)} |`);
		out.push('');
	}
	emit('src/content/docs/api/cpp/index.md', out.join('\n'));

	for (const c of cppClasses) {
		const o = [];
		o.push(frontmatter({ title: c.title, description: description(c.brief, `C++ class ${c.title}`), toc: [2, 2] }));
		o.push(GENERATED_NOTE(`${c.header}:${c.line}`));
		o.push(c.brief || `C++ type \`${c.title}\`.`);
		o.push('');
		if (c.detail) o.push(c.detail, '');
		o.push(`- **Header:** \`#include <${c.header}>\` ([source](${srcUrl(c.header.replace('sh4zam/', ''), c.line)}))`);
		if (c.ctype) o.push(`- **C type:** ${symbolLink(c.ctype) || '`' + c.ctype + '`'} (layout-compatible; the C API accepts it directly)`);
		if (c.bases.length) o.push(`- **Bases:** ${c.bases.map((b) => '`' + b + '`').join(', ')}`);
		o.push('');
		const members = [...c.members.values()];
		const groups = [
			['Static functions', members.filter((m) => m.static && m.sigs.some((s) => s.includes('(')))],
			['Members', members.filter((m) => !(m.static && m.sigs.some((s) => s.includes('('))))],
		];
		for (const [title, list] of groups) {
			if (!list.length) continue;
			o.push(`## ${title}\n`);
			for (const m of list) {
				o.push(`### ${m.name}\n`);
				o.push('```cpp');
				o.push(...m.sigs);
				o.push('```\n');
				if (m.brief) o.push(m.brief, '');
				if (m.resolved && m.resolved.length) o.push(`Calls: ${m.resolved.map((x) => symbolLink(x, x + '()')).join(', ')}${m.inherited ? ' (inherited from `shz::vecN`)' : ''}`, '');
				else if (m.inherited) o.push('Inherited from `shz::vecN`.', '');
			}
		}
		emit(`src/content/docs/api/cpp/${c.slug}.md`, o.join('\n'));
	}
}

// ---------------------------------------------------------------------------
// Cheatsheet

const shortSig = (s) =>
	s.signature
		.replace(/\b(SHZ_INLINE|SHZ_FORCE_INLINE|SHZ_NOEXCEPT|SHZ_CONST|SHZ_PURE|SHZ_HOT|SHZ_COLD|SHZ_FAST_MATH|SHZ_NO_INLINE|SHZ_DECLS_BEGIN|SHZ_DECLS_END)\b\s*/g, '')
		.replace(/\s+;/, ';')
		.replace(/;$/, '')
		.trim();

{
	const out = [];
	out.push(frontmatter({ title: 'Cheatsheet', description: 'The whole SH4ZAM C API on one page: every symbol with its signature and a one-line description', toc: [2, 2] }));
	out.push(GENERATED_NOTE('the public headers'));
	out.push(`Every public SH4ZAM ${VERSION} symbol on one page. Conventions: column-major matrices, quaternions stored \`<W, X, Y, Z>\`, radians by default, single-precision \`float\` only. C++ names drop the \`shz_\` prefix and live in \`namespace shz\`; see the [C++ API](/api/cpp/).`);
	out.push('');
	for (const mod of MODULES) {
		out.push(`## ${mod.title}\n`);
		out.push(`\`#include <sh4zam/${mod.file}>\` · [module page](/api/${mod.id}/)\n`);
		for (const sec of sectionOrder.get(mod.id) || []) {
			const rows = symbols.filter((s) => s.module === mod.id && s.section === sec && s.kind !== 'enumerator');
			if (!rows.length) continue;
			out.push(`### ${mod.title}: ${sec}\n`);
			for (const s of rows) {
				const sig = s.kind === 'type' || s.kind === 'enum' ? `${s.kind === 'enum' ? 'enum' : 'struct'} ${s.name}` : shortSig(s);
				out.push(`- [\`${s.name}\`](${s.path}): \`${sig.replace(/`/g, "'")}\`${s.brief ? ' ' + stripMdKeepCode(s.brief) : ''}`);
			}
			out.push('');
		}
	}
	emit('src/content/docs/cheatsheet.md', out.join('\n'));
}

function stripMdKeepCode(s) {
	return s.replace(/\[(`[^`]*`)\]\([^)]*\)/g, '$1').replace(/\n+/g, ' ');
}

// ---------------------------------------------------------------------------
// JSON

const pub = (s) => ({
	name: s.name,
	kind: s.kind,
	module: s.module,
	section: s.section,
	header: s.header,
	line: s.line,
	signature: s.signature,
	...(s.return_type ? { return_type: s.return_type } : {}),
	...(s.params ? { params: s.params } : {}),
	...(s.fields ? { fields: s.fields } : {}),
	brief: stripMd(s.brief) || 'No description in the upstream header.',
	detail: s.detail,
	returns: stripMd(s.returns || ''),
	notes: s.notes.map(stripMd),
	warnings: s.warnings.map(stripMd),
	sa: s.sa,
	documented: s.documented,
	...(s.deprecated ? { deprecated: stripMd(s.deprecated) } : {}),
	backends: s.backends || null,
	generic: s.generic || null,
	cpp: s.cpp || [],
	url: SITE + s.path,
	md: SITE + s.path.replace(/\/(#.*)?$/, '.md'),
	doxygen_url: s.documented && s._legacy ? `${SITE}/${s._legacy.page}${s._legacy.anchor ? '#' + s._legacy.anchor : ''}` : null,
	source_url: srcUrl(s.header.replace('sh4zam/', ''), s.line),
});

const moduleJson = (mod) => ({
	id: mod.id,
	title: mod.title,
	header: `sh4zam/${mod.file}`,
	cpp_header: `sh4zam/${mod.cpp_file}`,
	brief: stripMd(mod.brief),
	url: `${SITE}/api/${mod.id}/`,
	md: `${SITE}/api/${mod.id}.md`,
	symbol_count: symbols.filter((s) => s.module === mod.id).length,
	sections: sectionOrder.get(mod.id) || [],
});

const indexJson = {
	$schema: `${SITE}/api/schema.json`,
	library: 'sh4zam',
	version: VERSION,
	upstream: UPSTREAM,
	upstream_commit: COMMIT,
	generated: COMMIT_DATE,
	conventions: {
		c_prefix: 'shz_',
		cpp_namespace: 'shz',
		matrices: 'column-major',
		quaternion_order: 'W, X, Y, Z',
		angles: 'radians by default; _deg variants take degrees; u16 variants take 16-bit fixed point (65536 = 2*pi)',
		precision: 'single-precision float only',
		coordinates: 'right-handed world/view space, left-handed screen/clip space (as with GL)',
	},
	suffixes: Object.fromEntries(Object.entries(SUFFIXES).map(([k, v]) => [k, stripMd(v)])),
	modules: MODULES.map(moduleJson),
	cpp: {
		classes: cppClasses.map((c) => ({
			name: `shz::${c.name}`,
			ctype: c.ctype,
			header: c.header,
			url: `${SITE}/api/cpp/${c.slug}/`,
			members: [...c.members.values()].map((m) => ({ name: m.name, static: m.static, signatures: m.sigs, brief: stripMd(m.brief), calls: m.resolved || [] })),
		})),
		aliases: cppAliases.map((a) => ({ name: a.name, target: a.target })),
	},
	symbols: symbols.map(pub),
};

const schema = {
	$schema: 'http://json-schema.org/draft-07/schema#',
	$id: `${SITE}/api/schema.json`,
	title: 'SH4ZAM API index',
	description: 'Machine-readable index of every public SH4ZAM symbol. Generated from the upstream headers.',
	type: 'object',
	required: ['library', 'version', 'upstream_commit', 'generated', 'modules', 'symbols'],
	properties: {
		$schema: { type: 'string' },
		library: { const: 'sh4zam' },
		version: { type: 'string', pattern: '^\\d+\\.\\d+\\.\\d+$' },
		upstream: { type: 'string' },
		upstream_commit: { type: 'string', pattern: '^[0-9a-f]{40}$' },
		generated: { type: 'string', description: 'Commit date of the upstream revision the index was generated from.' },
		conventions: { type: 'object', additionalProperties: { type: 'string' } },
		suffixes: { type: 'object', additionalProperties: { type: 'string' } },
		modules: { type: 'array', items: { $ref: '#/definitions/module' } },
		cpp: { type: 'object' },
		symbols: { type: 'array', items: { $ref: '#/definitions/symbol' } },
	},
	definitions: {
		module: {
			type: 'object',
			required: ['id', 'title', 'header', 'brief', 'url', 'md'],
			properties: {
				id: { enum: MODULES.map((m) => m.id) },
				title: { type: 'string' },
				header: { type: 'string' },
				cpp_header: { type: 'string' },
				brief: { type: 'string', minLength: 1 },
				url: { type: 'string', format: 'uri' },
				md: { type: 'string', format: 'uri' },
				symbol_count: { type: 'integer' },
				sections: { type: 'array', items: { type: 'string' } },
			},
		},
		param: {
			type: 'object',
			required: ['name', 'type', 'doc'],
			properties: { name: { type: 'string' }, type: { type: 'string' }, doc: { type: 'string' } },
		},
		symbol: {
			type: 'object',
			required: ['name', 'kind', 'module', 'section', 'header', 'line', 'signature', 'brief', 'url', 'md', 'cpp', 'generic', 'backends', 'documented'],
			properties: {
				name: { type: 'string', pattern: '^(shz|SHZ)_[A-Za-z0-9_]+$' },
				kind: { enum: ['function', 'macro', 'type', 'typedef', 'enum', 'enumerator', 'variable'] },
				module: { enum: MODULES.map((m) => m.id) },
				section: { type: 'string' },
				header: { type: 'string', pattern: '^sh4zam/shz_\\w+\\.h$' },
				line: { type: 'integer', minimum: 1 },
				signature: { type: 'string', minLength: 1 },
				return_type: { type: 'string' },
				params: { type: 'array', items: { $ref: '#/definitions/param' } },
				fields: { type: 'array', items: { $ref: '#/definitions/param' } },
				brief: { type: 'string', minLength: 1 },
				detail: { type: 'string', description: 'Markdown.' },
				returns: { type: 'string' },
				notes: { type: 'array', items: { type: 'string' } },
				warnings: { type: 'array', items: { type: 'string' } },
				sa: { type: 'array', items: { type: 'string' }, description: 'See-also symbol names.' },
				documented: { type: 'boolean', description: 'False when the upstream header has no doc comment for this symbol.' },
				deprecated: { type: 'string' },
				backends: {
					description: 'Per back-end implementation. sh4: "asm" = dedicated SH4 implementation (inline asm, .s or FPU builtins), "c" = shared C code. spu: "simd" = dedicated SPU implementation. null for non-functions.',
					oneOf: [
						{ type: 'null' },
						{
							type: 'object',
							required: ['sh4', 'sw', 'spu'],
							properties: { sh4: { enum: ['asm', 'c'] }, sw: { enum: ['c'] }, spu: { enum: ['simd', 'c'] } },
						},
					],
				},
				generic: { type: ['string', 'null'], description: 'C type-generic macro that dispatches to this function.' },
				cpp: { type: 'array', items: { type: 'string' }, description: 'C++ names that wrap this symbol.' },
				url: { type: 'string', format: 'uri' },
				md: { type: 'string', format: 'uri' },
				doxygen_url: { type: ['string', 'null'], description: 'Legacy Doxygen URL on sh4zam.com (redirects to url).' },
				source_url: { type: 'string', format: 'uri' },
			},
		},
	},
};

const json = (o) => JSON.stringify(o, null, '\t');
emit('public/api/index.json', json(indexJson));
// Site chrome (header version badge, footer commit) reads this.
emit('src/data/upstream.json', json({ version: VERSION, commit: COMMIT, date: COMMIT_DATE, repo: UPSTREAM }));
emit('public/api/schema.json', json(schema));
for (const mod of MODULES) {
	emit(`public/api/${mod.id}.json`, json({ ...moduleJson(mod), library: 'sh4zam', version: VERSION, upstream_commit: COMMIT, symbols: symbols.filter((s) => s.module === mod.id).map(pub) }));
}

// Legacy Doxygen URLs -> new pages.
{
	const pages = {};
	const put = (page, target, anchor, anchorTarget) => {
		if (!pages[page]) pages[page] = { page: target, anchors: {} };
		if (anchor) pages[page].anchors[anchor] = anchorTarget;
	};
	for (const mod of MODULES) {
		const id = mod.file.replace(/_/g, '__').replace('.h', '_8h');
		put(`${id}.html`, `/api/${mod.id}/`);
		put(`${id}_source.html`, `/api/${mod.id}/`);
		put(`${mod.cpp_file.replace(/_/g, '__').replace('.hpp', '_8hpp')}.html`, '/api/cpp/');
		if (mod.id !== 'cdefs' && mod.id !== 'version') put(`group__${mod.id}.html`, `/api/${mod.id}/`);
	}
	for (const s of symbols) {
		if (!s._legacy || s.kind === 'enumerator') continue;
		if (s.kind === 'type') put(s._legacy.page, s.path);
		else put(s._legacy.page, `/api/${s.module}/`, s._legacy.anchor, s.path);
	}
	for (const c of cppClasses) {
		const id = c.name === 'version' ? 'classshz_1_1version' : `structshz_1_1${c.name.replace(/N$/, '_n')}`;
		put(`${id}.html`, `/api/cpp/${c.slug}/`);
	}
	put('namespaceshz.html', '/api/cpp/');
	put('guide.html', '/guides/getting-started/');
	put('tips.html', '/guides/optimization/');
	put('resources.html', '/resources/');
	put('todo.html', '/changelog/');
	put('files.html', '/api/');
	put('topics.html', '/api/');
	put('annotated.html', '/api/cpp/');
	put('classes.html', '/api/cpp/');
	put('globals.html', '/cheatsheet/');
	put('globals_func.html', '/cheatsheet/');
	put('globals_defs.html', '/cheatsheet/');
	put('globals_type.html', '/cheatsheet/');
	put('functions.html', '/api/cpp/');
	const sorted = Object.fromEntries(Object.keys(pages).sort().map((k) => [k, { page: pages[k].page, anchors: Object.fromEntries(Object.entries(pages[k].anchors).sort()) }]));
	emit('public/api/anchors.json', json({ generated_from: COMMIT, pages: sorted }));
	const lines = ['# Legacy Doxygen URLs -> new pages (Netlify / Cloudflare Pages). Generated by scripts/gen-api.mjs.'];
	for (const [k, v] of Object.entries(sorted)) lines.push(`/${k} ${v.page} 301`);
	emit('public/_redirects', lines.join('\n'));
}

// ---------------------------------------------------------------------------
// Reports

const undocumented = symbols.filter((s) => !s.documented && s.kind !== 'enumerator').map((s) => `${s.header}:${s.line}\t${s.kind}\t${s.name}`);
fs.mkdirSync(path.join(ROOT, 'build'), { recursive: true });
fs.writeFileSync(path.join(ROOT, 'build/undocumented.txt'), undocumented.join('\n') + '\n');
fs.writeFileSync(
	path.join(ROOT, 'build/unresolved-refs.txt'),
	[...unresolved.entries()].sort().map(([n, where]) => `${n}\t${[...where].sort().join(', ')}`).join('\n') + '\n' + unresolvedCpp.sort().map((l) => `cpp\t${l}`).join('\n') + '\n',
);

// ---------------------------------------------------------------------------
// Write or check

const OWNED = ['src/content/docs/api', 'public/api'];
const OWNED_FILES = ['src/content/docs/cheatsheet.md', 'public/_redirects', 'src/data/upstream.json'];
function listOwned() {
	const out = [...OWNED_FILES.filter((f) => fs.existsSync(path.join(ROOT, f)))];
	const walk = (d) => {
		const abs = path.join(ROOT, d);
		if (!fs.existsSync(abs)) return;
		for (const f of fs.readdirSync(abs)) {
			const rel = `${d}/${f}`;
			if (fs.statSync(path.join(ROOT, rel)).isDirectory()) walk(rel);
			else out.push(rel);
		}
	};
	OWNED.forEach(walk);
	return out;
}

const existing = listOwned();
const stale = existing.filter((f) => !files.has(f));
const changed = [...files.entries()].filter(([f, c]) => {
	const abs = path.join(ROOT, f);
	return !fs.existsSync(abs) || fs.readFileSync(abs, 'utf8').replace(/\r\n/g, '\n') !== c;
});

const counts = {
	symbols: symbols.length,
	pages: [...files.keys()].filter((f) => f.endsWith('.md')).length,
	undocumented: undocumented.length,
	unresolved_refs: unresolved.size,
	cpp_classes: cppClasses.length,
	cpp_aliases: cppAliases.length,
};

if (CHECK) {
	if (changed.length || stale.length) {
		console.error(`gen-api --check: generated output is out of date (${changed.length} changed, ${stale.length} stale).`);
		for (const [f] of changed.slice(0, 20)) console.error(`  changed: ${f}`);
		for (const f of stale.slice(0, 20)) console.error(`  stale:   ${f}`);
		console.error('Run `npm run gen` and commit the result.');
		process.exit(1);
	}
	console.log(`gen-api --check: up to date (${JSON.stringify(counts)})`);
} else {
	for (const f of stale) fs.rmSync(path.join(ROOT, f));
	for (const [f, c] of changed) {
		fs.mkdirSync(path.dirname(path.join(ROOT, f)), { recursive: true });
		fs.writeFileSync(path.join(ROOT, f), c);
	}
	console.log(`gen-api: wrote ${changed.length} files, removed ${stale.length} (${JSON.stringify(counts)})`);
}
