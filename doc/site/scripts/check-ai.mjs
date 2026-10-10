#!/usr/bin/env node
// Verifies the built site in dist/ (run after `npm run build`).
//   node scripts/check-ai.mjs              all checks, internal links only
//   node scripts/check-ai.mjs --external   also HEAD-check external links (warnings only)
// Respects SITE_URL the same way the build does, so it works on the Pages preview build.

import fs from 'node:fs';
import path from 'node:path';
import { execFileSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';
import Ajv from 'ajv';
import { checkIdentifiers } from './check-identifiers.mjs';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const DIST = path.join(ROOT, 'dist');
const siteUrl = new URL(process.env.SITE_URL || 'https://sh4zam.com');
const ORIGIN = siteUrl.origin;
const BASE = siteUrl.pathname.replace(/\/$/, '');
const SITE = ORIGIN + BASE;
const EXTERNAL = process.argv.includes('--external');

const failures = [];
const warnings = [];
const fail = (check, msg) => failures.push(`[${check}] ${msg}`);
const read = (p) => fs.readFileSync(p, 'utf8');

if (!fs.existsSync(DIST)) {
	console.error('check-ai: dist/ not found; run npm run build first');
	process.exit(1);
}

function walk(dir, out = []) {
	for (const f of fs.readdirSync(dir)) {
		const p = path.join(dir, f);
		if (fs.statSync(p).isDirectory()) walk(p, out);
		else out.push(p);
	}
	return out;
}
const all = walk(DIST);
const rel = (p) => path.relative(DIST, p).split(path.sep).join('/');
const isStub = (html) => html.includes('<meta name="robots" content="noindex">');
const pages = all.filter((p) => p.endsWith('index.html')).map((p) => ({ file: p, rel: rel(p), html: read(p) }));
const pagePath = (r) => '/' + r.replace(/index\.html$/, ''); // '' -> '/', 'api/' -> '/api/'
const twinOf = (r) => {
	const p = pagePath(r).replace(/\/$/, '');
	return path.join(DIST, `${p || '/index'}.md`);
};

// URL (absolute or root-relative) -> file in dist, or null when not ours.
function resolveUrl(u, fromRel = '') {
	let url;
	try {
		url = new URL(u, `${SITE}/${fromRel}`);
	} catch {
		return null;
	}
	if (url.origin !== ORIGIN) return null;
	let p = decodeURIComponent(url.pathname);
	if (BASE && !p.startsWith(BASE + '/') && p !== BASE) return { missingBase: true, file: p };
	p = p.slice(BASE.length) || '/';
	let file = path.join(DIST, p);
	if (p.endsWith('/')) file = path.join(file, 'index.html');
	return { file };
}

// 1. Markdown twin for every HTML page --------------------------------------
for (const pg of pages) {
	const twin = twinOf(pg.rel);
	if (!fs.existsSync(twin)) fail(1, `no Markdown twin for ${pg.rel} (expected ${rel(twin)})`);
	else {
		const t = read(twin);
		if (!t.trim()) fail(1, `empty twin ${rel(twin)}`);
		else if (!t.startsWith('# ')) fail(1, `twin ${rel(twin)} does not start with "# "`);
	}
}

// 2. llms.txt family ---------------------------------------------------------
const llms = path.join(DIST, 'llms.txt');
if (!fs.existsSync(llms)) fail(2, 'llms.txt missing');
else {
	const text = read(llms);
	const size = Buffer.byteLength(text);
	if (size > 60 * 1024) fail(2, `llms.txt is ${size} bytes (> 60 KB)`);
	const lines = text.split('\n');
	if (!/^# \S/.test(lines[0])) fail(2, 'llms.txt must start with an H1');
	if (!lines.some((l) => l.startsWith('> '))) fail(2, 'llms.txt has no "> " summary');
	if (!lines.some((l) => l.startsWith('## '))) fail(2, 'llms.txt has no "## " sections');
	const links = [...text.matchAll(/\]\((https?:[^)\s]+)\)/g)].map((m) => m[1]);
	if (links.length < 20) fail(2, `llms.txt has only ${links.length} links`);
	for (const l of links) {
		const r = resolveUrl(l);
		if (!r) continue; // external (upstream repo)
		if (r.missingBase || !fs.existsSync(r.file)) fail(2, `llms.txt link does not resolve: ${l}`);
	}
	const full = fs.existsSync(path.join(DIST, 'llms-full.txt')) ? read(path.join(DIST, 'llms-full.txt')) : '';
	if (!full) fail(2, 'llms-full.txt missing');
	if (!fs.existsSync(path.join(DIST, 'llms-api.txt'))) fail(2, 'llms-api.txt missing');
	for (const pg of pages) {
		const src = `Source: ${SITE}${pagePath(pg.rel)}`;
		if (full && !full.includes(src + '\n')) fail(2, `llms-full.txt lacks ${pagePath(pg.rel)}`);
	}
}

// 3. api/index.json -----------------------------------------------------------
{
	const index = JSON.parse(read(path.join(DIST, 'api/index.json')));
	const schema = JSON.parse(read(path.join(DIST, 'api/schema.json')));
	const ajv = new Ajv({ allErrors: true, strict: false, validateFormats: false });
	const validate = ajv.compile(schema);
	if (!validate(index)) for (const e of validate.errors.slice(0, 10)) fail(3, `index.json ${e.instancePath} ${e.message}`);
	for (const mod of index.modules) {
		const mj = JSON.parse(read(path.join(DIST, `api/${mod.id}.json`)));
		if (mj.symbols.length !== index.symbols.filter((s) => s.module === mod.id).length) fail(3, `api/${mod.id}.json symbol count mismatch`);
	}
	// Grep count from public headers minus \todo names.
	const inc = path.join(ROOT, '../../include/sh4zam');
	const names = new Set();
	const todo = new Set();
	for (const f of fs.readdirSync(inc).filter((f) => f.endsWith('.h'))) {
		const s = read(path.join(inc, f));
		for (const m of s.matchAll(/shz_[a-z0-9_]+(?=\()/g)) names.add(m[0]);
		for (const block of s.matchAll(/\\todo([\s\S]*?)(?:\n\s*\*?\s*\n|\*\/)/g)) for (const m of block[1].matchAll(/shz_[a-z0-9_]+/g)) todo.add(m[0]);
	}
	// Stale names in older header prose: referenced, never declared.
	const stale = ['shz_xmtrx_trans_vec2', 'shz_xmtrx_trans_vec3', 'shz_xmtrx_trans_vec4', 'shz_xmtrx_load_apply'];
	const expected = [...names].filter((n) => !todo.has(n) && !stale.includes(n));
	const have = new Set(index.symbols.map((s) => s.name));
	const missing = expected.filter((n) => !have.has(n));
	if (missing.length) fail(3, `index.json lacks ${missing.length} header names: ${missing.slice(0, 10).join(', ')} (a new header needs an entry in MODULES in scripts/data/modules.mjs; see README.md)`);
	if (index.symbols.length < expected.length) fail(3, `index.json has ${index.symbols.length} symbols < ${expected.length} grepped`);
	const noBrief = index.symbols.filter((s) => !s.brief || !s.brief.trim());
	if (noBrief.length) fail(3, `${noBrief.length} symbols without brief: ${noBrief.slice(0, 5).map((s) => s.name).join(', ')}`);
	for (const s of index.symbols) {
		const r = resolveUrl(s.url.replace('https://sh4zam.com', SITE));
		if (r && (r.missingBase || !fs.existsSync(r.file.split('#')[0]))) fail(3, `symbol url does not resolve: ${s.url}`);
	}
}

// 4. Per-page head tags -------------------------------------------------------
for (const pg of pages) {
	if (pg.rel === '404.html') continue;
	const h = pg.html;
	const need = [
		['title', /<title>[^<]+<\/title>/],
		['description', /<meta name="description" content="[^"]+"/],
		['canonical', /<link rel="canonical" href="[^"]+"/],
		['JSON-LD', /<script type="application\/ld\+json">/],
		['markdown alternate', /<link rel="alternate" type="text\/markdown" href="[^"]+\.md"/],
		['lang', /<html[^>]* lang="en"/],
	];
	for (const [what, re] of need) if (!re.test(h)) fail(4, `${pg.rel}: missing ${what}`);
	const d = h.match(/<meta name="description" content="([^"]*)"/);
	if (d && d[1].length > 160) fail(4, `${pg.rel}: description longer than 160 chars`);
	if ((h.match(/<h1[\s>]/g) || []).length !== 1) fail(4, `${pg.rel}: expected exactly one <h1>`);
}

// 5. robots.txt + sitemap -----------------------------------------------------
{
	const robots = fs.existsSync(path.join(DIST, 'robots.txt')) ? read(path.join(DIST, 'robots.txt')) : '';
	const bots = ['GPTBot', 'ChatGPT-User', 'OAI-SearchBot', 'ClaudeBot', 'Claude-User', 'Claude-SearchBot', 'anthropic-ai', 'PerplexityBot', 'Google-Extended', 'Applebot-Extended', 'CCBot', 'Bytespider', 'Amazonbot', 'meta-externalagent', 'DuckAssistBot', 'cohere-ai', 'YouBot'];
	if (!/User-agent: \*\nAllow: \//.test(robots)) fail(5, 'robots.txt must allow User-agent: *');
	for (const b of bots) if (!robots.includes(`User-agent: ${b}\nAllow: /`)) fail(5, `robots.txt lacks Allow for ${b}`);
	if (!robots.includes(`Sitemap: ${SITE}/sitemap-index.xml`)) fail(5, 'robots.txt lacks Sitemap line');
	const locs = new Set();
	for (const f of all.filter((p) => /sitemap-\d+\.xml$/.test(p))) for (const m of read(f).matchAll(/<loc>([^<]+)<\/loc>/g)) locs.add(m[1]);
	for (const pg of pages) if (pg.rel !== '404.html' && !locs.has(`${SITE}${pagePath(pg.rel)}`)) fail(5, `sitemap lacks ${pagePath(pg.rel)}`);
}

// 6. Banned phrases + AI mentions ---------------
{
	const banned = [
		/\bWelcome to\b/, /get started in seconds/i, /\bseamless(ly)?\b/i, /\brobust\b/i, /\bpowerful\b/i, /blazing[ -]fast/i,
		/\bmodern\b/i, /\bdelightful\b/i, /\beffortless(ly)?\b/i, /\bcomprehensive\b/i, /in this guide,? we will/i, /—/,
		/\b(Claude|ChatGPT|Copilot)\b/, /\bAI[- ](generated|assisted|assistance)\b/i, /\bgenerated (by|with) AI\b/i, /\blorem ipsum\b/i,
	];
	const emoji = /[\u{1F300}-\u{1FAFF}\u{2600}-\u{27BF}\u{1F000}-\u{1F2FF}]/u;
	// Visible text only: drop scripts, styles and tags, so CSS/JS identifiers don't count.
	const visible = (html) =>
		html
			.replace(/<script[\s\S]*?<\/script>/g, ' ')
			.replace(/<style[\s\S]*?<\/style>/g, ' ')
			.replace(/<svg[\s\S]*?<\/svg>/g, ' ')
			.replace(/<[^>]+>/g, ' ');
	const targets = [
		...pages.map((p) => [p.rel, visible(p.html)]),
		...all.filter((p) => p.endsWith('.md')).map((p) => [rel(p), read(p)]),
		...['llms.txt'].map((f) => [f, read(path.join(DIST, f))]),
	];
	for (const [name, text] of targets) {
		for (const re of banned) {
			const m = text.match(re);
			if (m) fail(6, `${name}: banned phrase "${m[0]}"`);
		}
		if (emoji.test(text)) fail(6, `${name}: emoji`);
	}
}

// 7. Links ------------------------------------------------------------------
const external = new Set();
{
	let checked = 0;
	for (const f of all.filter((p) => p.endsWith('.html'))) {
		const r = rel(f);
		const html = read(f);
		if (isStub(html) || r === '404.html') continue; // stubs redirect; 404 has no canonical page
		for (const m of html.matchAll(/\s(?:href|src)="([^"]+)"/g)) {
			const u = m[1].replace(/&amp;/g, '&');
			if (/^(mailto:|javascript:|data:|#)/.test(u)) continue;
			if (/^https?:\/\//.test(u) && !u.startsWith(ORIGIN)) {
				external.add(u.split('#')[0]);
				continue;
			}
			const res = resolveUrl(u, r.replace(/index\.html$/, ''));
			if (!res) continue;
			checked++;
			const file = res.file.split('#')[0];
			if (res.missingBase || !fs.existsSync(file)) fail(7, `${r}: broken link ${u}`);
		}
	}
	// Links inside Markdown twins (absolute URLs).
	for (const f of all.filter((p) => p.endsWith('.md'))) {
		for (const m of read(f).matchAll(/\]\((https?:[^)\s]+)\)/g)) {
			const res = resolveUrl(m[1]);
			if (!res) {
				external.add(m[1].split('#')[0]);
				continue;
			}
			checked++;
			if (res.missingBase || !fs.existsSync(res.file.split('#')[0])) fail(7, `${rel(f)}: broken link ${m[1]}`);
		}
	}
	console.log(`check-ai: ${pages.length} pages, ${checked} internal links checked, ${external.size} external links${EXTERNAL ? '' : ' (not fetched; pass --external)'}`);
}

if (EXTERNAL) {
	const list = [...external].filter((u) => !/^https:\/\/github\.com\/[^/]+\/[^/]+\/edit\//.test(u)).sort();
	const bad = [];
	await Promise.all(
		list.map(async (u, i) => {
			await new Promise((r) => setTimeout(r, (i % 20) * 50));
			try {
				let res = await fetch(u, { method: 'HEAD', redirect: 'follow', signal: AbortSignal.timeout(15000) });
				if (res.status === 405 || res.status === 403) res = await fetch(u, { redirect: 'follow', signal: AbortSignal.timeout(15000) });
				if (res.status >= 400) bad.push(`${res.status} ${u}`);
			} catch (e) {
				bad.push(`ERR ${u} (${e.cause?.code || e.name})`);
			}
		}),
	);
	for (const b of bad.sort()) warnings.push(`[7] external: ${b}`);
	console.log(`check-ai: ${list.length} external links fetched, ${bad.length} warnings`);
}

// Handwritten pages name only real symbols.
for (const p of checkIdentifiers()) fail('ids', p);

// Generated API pages match the headers (skipped when doxygen isn't installed).
if (process.argv.includes('--gen-check')) {
	try {
		execFileSync(process.execPath, ['scripts/gen-api.mjs', '--check'], { cwd: ROOT, stdio: 'inherit' });
	} catch {
		fail('gen', 'generated output is stale');
	}
}

for (const w of warnings) console.warn(`warn ${w}`);
for (const f of failures.slice(0, 200)) console.error(`FAIL ${f}`);
if (failures.length > 200) console.error(`... ${failures.length - 200} more`);
console.log(`check-ai: ${failures.length ? `${failures.length} failure(s)` : 'all checks passed'}${warnings.length ? `, ${warnings.length} warning(s)` : ''}`);
process.exit(failures.length ? 1 : 0);
