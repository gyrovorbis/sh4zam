#!/usr/bin/env node
// Every shz_* / SHZ_* / shz:: name mentioned in a handwritten page must exist in the
// generated API index (public/api/index.json). Generated pages are skipped.
// Exit 1 on unknown names. Used by scripts/check-ai.mjs; runnable on its own.

import fs from 'node:fs';
import path from 'node:path';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const DOCS = path.join(ROOT, 'src/content/docs');

// Names that are deliberately mentioned as *not* existing, or
// that only occur in prose as patterns.
const ALLOW = new Set([
	'shz_memset', 'shz_memset2', 'shz_memset4', 'shz_memset32', 'shz_macw',
	'shz_xmtrx_trans_vec4', 'shz_xmtrx_load_4x4_wxyz', 'shz_xmtrx_angles', 'shz_xmtrx_position', 'shz_xmtrx_size',
	'shz_foo_sh4', 'shz_foo_sw', 'shz_xmtrx_reg', 'SHZ_X',
	// CMake options, not C identifiers.
	'SHZ_ENABLE_TESTS', 'SHZ_SAVE_TEMPS', 'SHZ_ENABLE_PIC', 'SHZ_LIBRARY_TYPE', 'SHZ_DISABLE_BENCHMARKS',
]);
const PATTERN_PREFIX = /^(shz|SHZ)_[A-Za-z0-9]*$|_$/; // `shz_vec3_*`, `shz_` style prefixes

export function checkIdentifiers() {
	const index = JSON.parse(fs.readFileSync(path.join(ROOT, 'public/api/index.json'), 'utf8'));
	const names = new Set(index.symbols.map((s) => s.name));
	const cpp = new Set(['shz']);
	for (const c of index.cpp.classes) {
		cpp.add(c.name);
		for (const m of c.members) cpp.add(`${c.name}::${m.name}`);
	}
	for (const a of index.cpp.aliases) cpp.add(a.name);
	const consts = fs.readFileSync(path.join(DOCS, 'api/cpp/index.md'), 'utf8').matchAll(/`(shz::\w+)`/g);
	for (const m of consts) cpp.add(m[1]);

	const files = [];
	const walk = (d) => {
		for (const f of fs.readdirSync(d)) {
			const p = path.join(d, f);
			const rel = path.relative(DOCS, p).replace(/\\/g, '/');
			if (fs.statSync(p).isDirectory()) {
				if (rel !== 'api') walk(p);
			} else if (/\.mdx?$/.test(f) && rel !== 'cheatsheet.md') files.push(p);
		}
	};
	walk(DOCS);

	const problems = [];
	for (const f of files) {
		const text = fs.readFileSync(f, 'utf8');
		const rel = path.relative(ROOT, f).replace(/\\/g, '/');
		for (const m of text.matchAll(/\b((?:shz|SHZ)_[A-Za-z0-9_]+)(\.(?:h|hpp|c|cpp|s|inl)\b)?(\*)?/g)) {
			const [, name, ext, star] = m;
			if (ext || star || PATTERN_PREFIX.test(name) || ALLOW.has(name) || names.has(name)) continue;
			// shz_<module>.h style placeholders, shz_<name>_t patterns
			if (/<|>/.test(text.slice(m.index - 1, m.index + name.length + 1))) continue;
			problems.push(`${rel}: unknown C identifier ${name}`);
		}
		for (const m of text.matchAll(/\b(shz::[A-Za-z_][\w]*(?:::[A-Za-z_~][\w]*)?)/g)) {
			const name = m[1];
			if (cpp.has(name)) continue;
			problems.push(`${rel}: unknown C++ name ${name}`);
		}
	}
	return [...new Set(problems)];
}

if (process.argv[1] && path.resolve(process.argv[1]) === fileURLToPath(import.meta.url)) {
	const problems = checkIdentifiers();
	for (const p of problems) console.error(p);
	console.log(`check-identifiers: ${problems.length ? problems.length + ' problem(s)' : 'ok'}`);
	process.exit(problems.length ? 1 : 0);
}
