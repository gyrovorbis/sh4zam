// Minimal static server for dist/ (screenshots, link checks, local QA). Mirrors GitHub
// Pages: directory -> index.html, .md served as text/markdown, 404.html on miss.
import http from 'node:http';
import fs from 'node:fs';
import path from 'node:path';
import zlib from 'node:zlib';

const TYPES = {
	'.html': 'text/html; charset=utf-8', '.md': 'text/markdown; charset=utf-8', '.txt': 'text/plain; charset=utf-8',
	'.json': 'application/json', '.css': 'text/css', '.js': 'text/javascript', '.svg': 'image/svg+xml',
	'.png': 'image/png', '.ico': 'image/x-icon', '.woff2': 'font/woff2', '.woff': 'font/woff', '.xml': 'application/xml',
	'.webp': 'image/webp', '.pf_meta': 'application/octet-stream', '.pf_fragment': 'application/octet-stream',
	'.pf_index': 'application/octet-stream', '.pagefind': 'application/octet-stream', '.wasm': 'application/wasm',
};

export function serve(root = 'dist', port = 0, base = '/') {
	const server = http.createServer((req, res) => {
		let url = decodeURIComponent(new URL(req.url, 'http://x').pathname);
		if (base !== '/' && url.startsWith(base)) url = url.slice(base.length - 1);
		let file = path.join(root, url);
		if (fs.existsSync(file) && fs.statSync(file).isDirectory()) file = path.join(file, 'index.html');
		if (!fs.existsSync(file)) {
			res.writeHead(404, { 'content-type': TYPES['.html'] });
			return res.end(fs.existsSync(path.join(root, '404.html')) ? fs.readFileSync(path.join(root, '404.html')) : 'not found');
		}
		const type = TYPES[path.extname(file)] || 'application/octet-stream';
		// Compress text like GitHub Pages / most static hosts do.
		if (/text|json|javascript|svg|xml/.test(type) && /gzip/.test(req.headers['accept-encoding'] || '')) {
			res.writeHead(200, { 'content-type': type, 'content-encoding': 'gzip', 'cache-control': 'max-age=600' });
			return fs.createReadStream(file).pipe(zlib.createGzip()).pipe(res);
		}
		res.writeHead(200, { 'content-type': type, 'cache-control': 'max-age=600' });
		fs.createReadStream(file).pipe(res);
	});
	return new Promise((resolve) => server.listen(port, '127.0.0.1', () => resolve(server)));
}

if (process.argv[1] && process.argv[1].endsWith('serve.mjs')) {
	const port = Number(process.argv[2] || 4321);
	serve('dist', port).then(() => console.log(`serving dist/ on http://127.0.0.1:${port}/`));
}
