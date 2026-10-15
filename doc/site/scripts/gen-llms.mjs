#!/usr/bin/env node
// Post-build step (runs after `astro build`, see package.json):
//   - dist/llms.txt, dist/llms-full.txt, dist/llms-api.txt   (llmstxt.org format)
//   - dist/robots.txt
//   - legacy Doxygen URL stubs (shz__vector_8h.html#a... -> new page) from public/api/anchors.json
//   - when SITE_URL is not https://sh4zam.com (the GitHub Pages preview), rewrites the
//     canonical URLs inside dist/api/*.json so the preview is self-consistent.
// Everything here is derived from dist/ and the content sources; nothing is hand-edited.

import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const DIST = path.join(ROOT, 'dist');
const DOCS = path.join(ROOT, 'src/content/docs');
const CANON = 'https://sh4zam.com';
const siteUrl = new URL(process.env.SITE_URL || CANON);
const SITE = `${siteUrl.origin}${siteUrl.pathname.replace(/\/$/, '')}`; // no trailing slash
const BASE = siteUrl.pathname.replace(/\/$/, ''); // '' or a path prefix such as '/sh4zam'

if (!fs.existsSync(DIST)) {
	console.error('gen-llms: dist/ not found; run astro build first');
	process.exit(1);
}

const read = (p) => fs.readFileSync(p, 'utf8');
const write = (rel, s) => {
	const p = path.join(DIST, rel);
	fs.mkdirSync(path.dirname(p), { recursive: true });
	fs.writeFileSync(p, s);
};

// ---------------------------------------------------------------------------
// Preview: point JSON URLs at the preview host.

if (SITE !== CANON) {
	for (const f of fs.readdirSync(path.join(DIST, 'api')).filter((f) => f.endsWith('.json'))) {
		const p = path.join(DIST, 'api', f);
		fs.writeFileSync(p, read(p).replaceAll(`${CANON}/`, `${SITE}/`));
	}
}

// ---------------------------------------------------------------------------
// Page metadata from the content sources.

function frontmatter(file) {
	const m = read(file).match(/^---\n([\s\S]*?)\n---/);
	const fm = {};
	if (!m) return fm;
	for (const line of m[1].split('\n')) {
		const kv = line.match(/^(\w+):\s*(.*)$/);
		if (kv) fm[kv[1]] = kv[2].replace(/^"(.*)"$/, '$1').replace(/\\"/g, '"');
	}
	return fm;
}
const page = (id) => {
	const f = ['.md', '.mdx', '/index.md'].map((e) => path.join(DOCS, id + e)).find((p) => fs.existsSync(p));
	const fm = frontmatter(f);
	return { id, title: fm.title, description: fm.description, md: `${SITE}/${id}.md` };
};
const link = (p, label = p.title) => `- [${label}](${p.md}): ${p.description}`;

const index = JSON.parse(read(path.join(DIST, 'api/index.json')));

// ---------------------------------------------------------------------------
// llms.txt

const START = ['for-agents', 'cheatsheet', 'concepts/naming-and-suffixes', 'concepts/conventions', 'concepts/xmtrx', 'guides/matrix-transforms', 'guides/interop', 'guides/getting-started'].map(page);
const GUIDES = ['guides/getting-started', 'guides/install-kallistios', 'guides/install-cmake', 'guides/using-in-a-project', 'guides/interop', 'guides/matrix-transforms', 'guides/optimization', 'guides/examples', 'guides/testing-and-contributing'].map(page);
const CONCEPTS = ['concepts/architecture', 'concepts/naming-and-suffixes', 'concepts/types-and-layout', 'concepts/conventions', 'concepts/xmtrx', 'concepts/sh4-fpu', 'concepts/c-and-cpp', 'concepts/backends'].map(page);
const MORE = ['showcase', 'resources', 'community', 'contributing', 'changelog'].map(page);

const llms = [
	'# SH4ZAM',
	'',
	"> Fast math library for the Sega Dreamcast's SH4 CPU. Hand-optimized C17/C++20 linear algebra, trig, scalar, complex/FFT and memory routines using FIPR/FTRV/FSCA/FSRRA and the XMTRX register bank, with a portable software back-end.",
	'',
	`Version ${index.version}, generated from upstream commit ${index.upstream_commit.slice(0, 7)} (${index.generated}). Source: ${index.upstream}. MIT license.`,
	'',
	'Key facts: C prefix `shz_`, macros `SHZ_`, C++ namespace `shz::` with the `shz_` prefix dropped (`shz_sinf` -> `shz::sinf`, `shz_vec3_t` -> `shz::vec3`, `shz_quat_slerp` -> `shz::quat::slerp`, `shz_xmtrx_*` -> static members of `shz::xmtrx`). Column-major matrices; `shz_mat4x4_t` must be 8-byte aligned (`_unaligned` variants take `float[16]`). Quaternions stored W,X,Y,Z (W first). Radians by default. Single-precision `float` only. Right-handed world/view, left-handed clip space, like GL. XMTRX = the SH4 FPU back-bank 4x4 matrix: `shz_xmtrx_*` functions operate on that global (per-thread) register state, no matrix argument. Include `<sh4zam/shz_sh4zam.h>` (C) or `<sh4zam/shz_sh4zam.hpp>` (C++), link `-lsh4zam`.',
	'',
	'Suffixes: `_fsrra` (fast FSRRA approximation; argument/denominator must be positive), `_safe` (guards zero length/denominator), `_deg` (degrees), `u16` (16-bit fixed-point angle), `_unaligned` (no 8-byte alignment needed), `_transpose`, `_reverse` (reversed multiply order), `_wxyz` (W first out of FTRV), `_xmtrx` (memory routine that clobbers XMTRX), `sq_` (store queues). Matrix verbs: `init_*` (whole matrix), `set_*` (only those components), `apply_*` (to affected components), GL-style `translate/scale/rotate_*` (full multiply).',
	'',
	'Do not invent: `shz_memset()` does not exist (use `shz_memset8()`); `shz_xmtrx_trans_vec4()` is an old name for `shz_xmtrx_transform_vec4()`. Every symbol is listed in the JSON index; if it is not there, it is not public API.',
	'',
	'URL patterns: page `/<path>/`, Markdown twin `/<path>.md`; symbol `/api/<module>/<name>.md` with the exact C identifier.',
	'',
	'## Start here',
	'',
	...START.map((p) => link(p)),
	'',
	'## API reference (one page per module)',
	'',
	...index.modules.map((m) => `- [${m.id}](${SITE}/api/${m.id}.md): ${m.brief} \`${m.header}\`, ${m.symbol_count} symbols.`),
	`- [C++ API](${SITE}/api/cpp.md): classes \`${index.cpp.classes.map((c) => c.name).join('`, `')}\` and the free-function aliases, each mapped to the C function it calls.`,
	`- [API overview](${SITE}/api.md): module table and how to navigate.`,
	'',
	'## Guides',
	'',
	...GUIDES.map((p) => link(p)),
	'',
	'## Concepts',
	'',
	...CONCEPTS.map((p) => link(p)),
	'',
	'## More',
	'',
	...MORE.map((p) => link(p)),
	'',
	'## Optional',
	'',
	`- [llms-full.txt](${SITE}/llms-full.txt): every page on this site concatenated as Markdown`,
	`- [llms-api.txt](${SITE}/llms-api.txt): every API page (all ${index.symbols.length} symbols) concatenated`,
	`- [api/index.json](${SITE}/api/index.json): machine-readable symbol index ([schema](${SITE}/api/schema.json)); per-module files at \`/api/<module>.json\``,
	`- [Upstream repository](${index.upstream}): headers in \`include/sh4zam/\`, generated from \`${index.upstream_commit}\``,
	'',
].join('\n');
write('llms.txt', llms);

// ---------------------------------------------------------------------------
// llms-full.txt / llms-api.txt in sitemap order

const sitemapUrls = [];
for (const f of fs.readdirSync(DIST).filter((f) => /^sitemap-\d+\.xml$/.test(f)).sort()) {
	for (const m of read(path.join(DIST, f)).matchAll(/<loc>([^<]+)<\/loc>/g)) sitemapUrls.push(m[1]);
}
const twinFor = (url) => {
	let p = new URL(url).pathname;
	if (BASE && p.startsWith(BASE)) p = p.slice(BASE.length);
	p = p.replace(/\/$/, '');
	return path.join(DIST, `${p || '/index'}.md`);
};
const full = [];
const api = [];
let missing = 0;
for (const url of sitemapUrls) {
	const twin = twinFor(url);
	if (!fs.existsSync(twin)) {
		missing++;
		continue;
	}
	const chunk = `\n\n---\n${read(twin).trim()}\n`;
	full.push(chunk);
	if (new URL(url).pathname.slice(BASE.length).startsWith('/api/')) api.push(chunk);
}
const preamble = (what) => `# SH4ZAM: ${what}\n\nSource: ${SITE}/\nGenerated from upstream commit ${index.upstream_commit} (v${index.version}). Each section below is one page; its URL is on the line after its title.\n`;
write('llms-full.txt', preamble('full documentation') + full.join(''));
write('llms-api.txt', preamble('API reference') + api.join(''));

// ---------------------------------------------------------------------------
// robots.txt

const BOTS = ['GPTBot', 'ChatGPT-User', 'OAI-SearchBot', 'ClaudeBot', 'Claude-User', 'Claude-SearchBot', 'anthropic-ai', 'PerplexityBot', 'Google-Extended', 'Applebot-Extended', 'CCBot', 'Bytespider', 'Amazonbot', 'meta-externalagent', 'DuckAssistBot', 'cohere-ai', 'YouBot'];
write(
	'robots.txt',
	['User-agent: *', 'Allow: /', '', ...BOTS.flatMap((b) => [`User-agent: ${b}`, 'Allow: /', '']), `Sitemap: ${SITE}/sitemap-index.xml`, ''].join('\n'),
);

// ---------------------------------------------------------------------------
// Legacy Doxygen URL stubs

const anchors = JSON.parse(read(path.join(ROOT, 'public/api/anchors.json'))).pages;
let stubs = 0;
for (const [file, { page: target, anchors: map }] of Object.entries(anchors)) {
	const out = path.join(DIST, file);
	if (fs.existsSync(out)) continue; // never shadow a real page
	const to = `${BASE}${target}`;
	const local = Object.fromEntries(Object.entries(map).map(([a, t]) => [a, `${BASE}${t}`]));
	const html = `<!doctype html>
<html lang="en">
<head>
<meta charset="utf-8">
<title>Moved: ${file}</title>
<meta name="robots" content="noindex">
<link rel="canonical" href="${SITE}${target}">
<meta http-equiv="refresh" content="0; url=${to}">
<script>
var m=${JSON.stringify(local)};var h=location.hash.slice(1);location.replace(m[h]||${JSON.stringify(to)});
</script>
</head>
<body><p>This page moved to <a href="${to}">${SITE}${target}</a>.</p></body>
</html>
`;
	fs.writeFileSync(out, html);
	stubs++;
}

const kb = (f) => (fs.statSync(path.join(DIST, f)).size / 1024).toFixed(1);
console.log(
	`gen-llms: llms.txt ${kb('llms.txt')} KB, llms-full.txt ${kb('llms-full.txt')} KB (${full.length} pages), llms-api.txt ${kb('llms-api.txt')} KB (${api.length} pages), ${stubs} Doxygen stubs${missing ? `, ${missing} sitemap URLs without twin` : ''}`,
);
