// lastmod for the sitemap: generated API pages carry the upstream commit date, handwritten
// pages their last git commit date in this repo.
import fs from 'node:fs';
import path from 'node:path';
import { execFileSync } from 'node:child_process';

export function lastmodMap(root) {
	const docs = path.join(root, 'src/content/docs');
	const upstream = JSON.parse(fs.readFileSync(path.join(root, 'src/data/upstream.json'), 'utf8'));
	const map = new Map();
	const walk = (dir) => {
		for (const f of fs.readdirSync(dir)) {
			const p = path.join(dir, f);
			if (fs.statSync(p).isDirectory()) {
				walk(p);
				continue;
			}
			if (!/\.mdx?$/.test(f)) continue;
			const rel = path.relative(docs, p).split(path.sep).join('/');
			const fm = fs.readFileSync(p, 'utf8').match(/^slug: "([^"]+)"/m);
			const id = fm ? fm[1] : rel.replace(/\.mdx?$/, '').replace(/(^|\/)index$/, '').toLowerCase();
			const url = id ? `/${id}/` : '/';
			let date;
			if (rel.startsWith('api/') || rel === 'cheatsheet.md') date = upstream.date;
			else {
				try {
					date = execFileSync('git', ['log', '-1', '--format=%cs', '--', p], { cwd: root, encoding: 'utf8' }).trim();
				} catch {}
			}
			if (date) map.set(url, date);
		}
	};
	walk(docs);
	return map;
}
