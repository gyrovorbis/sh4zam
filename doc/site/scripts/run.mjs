#!/usr/bin/env node
// One-step local preview, used by run.cmd (Windows) and run.sh (macOS/Linux).
//
//   node scripts/run.mjs           check tools, install deps if needed, live preview (npm run dev)
//   node scripts/run.mjs --build   same, but a full production build served from dist/
//
// Checks for Node 20+ and Doxygen, runs `npm ci` when node_modules is missing or older
// than package-lock.json, then opens the site in the browser. Exits with 2 when a check,
// the install or the build fails, so run.cmd knows to keep its window open.

import fs from 'node:fs';
import path from 'node:path';
import { spawn, spawnSync } from 'node:child_process';
import { fileURLToPath } from 'node:url';

const ROOT = path.resolve(path.dirname(fileURLToPath(import.meta.url)), '..');
const WIN = process.platform === 'win32';
const MAC = process.platform === 'darwin';
const PORT = 4321;

function fail(msg) {
	console.error(`\n${msg}\n`);
	process.exit(2);
}

// npm is npm.cmd on Windows, which spawn can only start through a shell. The arguments
// are fixed strings from this file, so passing one command line is safe.
function npm(args) {
	return spawnSync(['npm', ...args].join(' '), { cwd: ROOT, stdio: 'inherit', shell: true });
}

function npmOrFail(args) {
	const res = npm(args);
	if (res.signal) process.exit(130); // Ctrl+C
	if (res.status !== 0) fail(`npm ${args.join(' ')} failed; see the output above.`);
}

const major = Number(process.versions.node.split('.')[0]);
if (major < 20) {
	fail(`Node.js ${process.version} is too old; 20 or newer is needed.\n` +
		(WIN ? 'Update with: winget install OpenJS.NodeJS.LTS' : MAC ? 'Update with: brew install node' : 'Get it from https://nodejs.org/'));
}

// Same lookup as gen-api.mjs, plus the Doxygen.app bundle on macOS. Whatever is found
// goes into DOXYGEN so the generator uses it too. An explicit DOXYGEN is never swapped
// for another install.
const works = (exe) => spawnSync(exe, ['--version']).status === 0;
if (process.env.DOXYGEN && !works(process.env.DOXYGEN)) {
	fail(`DOXYGEN is set to ${process.env.DOXYGEN}, but that doesn't run. Fix or unset it.`);
}
const doxygen = process.env.DOXYGEN || [
	WIN && 'C:/Program Files/doxygen/bin/doxygen.exe',
	MAC && '/Applications/Doxygen.app/Contents/Resources/doxygen',
	'doxygen',
].filter(Boolean).find(works);
if (!doxygen) {
	fail('Doxygen was not found.\n' +
		(WIN ? 'Install it with: winget install DimitriVanHeesch.Doxygen\n' : MAC ? 'Install it with: brew install doxygen\n' : 'Install it with your package manager.\n') +
		'Or set DOXYGEN to the full path of the doxygen executable.');
}
process.env.DOXYGEN = doxygen;

const lock = path.join(ROOT, 'package-lock.json');
const installed = path.join(ROOT, 'node_modules/.package-lock.json');
if (!fs.existsSync(installed) || fs.statSync(installed).mtimeMs < fs.statSync(lock).mtimeMs) {
	console.log('Installing dependencies (npm ci)...');
	npmOrFail(['ci']);
}

if (!process.argv.includes('--build')) {
	console.log('Starting the live preview. The first start takes a little while; Ctrl+C stops it.');
	// However the dev server ends (usually Ctrl+C), that's a normal stop.
	npm(['run', 'dev', '--', '--port', String(PORT), '--open']);
	process.exit(0);
}

// The local server has no path prefix, so build for the root even if SITE_URL is set.
delete process.env.SITE_URL;
npmOrFail(['run', 'build']);
const { serve } = await import('./serve.mjs');
const dist = path.join(ROOT, 'dist');
// If the port is taken (say the dev server is running), take any free one.
const server = await serve(dist, PORT).catch(() => serve(dist, 0));
const url = `http://127.0.0.1:${server.address().port}/`;
console.log(`Serving dist/ at ${url} (Ctrl+C stops it)`);
const [cmd, ...args] = WIN ? ['cmd', '/c', 'start', '', url] : [MAC ? 'open' : 'xdg-open', url];
spawn(cmd, args, { stdio: 'ignore', detached: true, windowsHide: true }).on('error', () => {}).unref();
