#!/usr/bin/env node
// Screenshots of representative pages at desktop and mobile widths, plus a horizontal
// overflow check. Run after `npm run build`. Output: build/shots/*.png
import fs from 'node:fs';
import { chromium } from 'playwright';
import { serve } from './serve.mjs';

const PAGES = {
	home: '/',
	module: '/api/vector/',
	symbol: '/api/xmtrx/shz_xmtrx_load_wxyz_4x4/',
	guide: '/guides/matrix-transforms/',
	cpp: '/api/cpp/quat/',
	search: '/guides/getting-started/',
};
const VIEWPORTS = { desktop: { width: 1440, height: 900 }, mobile: { width: 390, height: 844 } };

const out = process.argv[2] || 'build/shots';
fs.mkdirSync(out, { recursive: true });
const server = await serve('dist', 0);
const origin = `http://127.0.0.1:${server.address().port}`;
const browser = await chromium.launch();
const problems = [];
for (const [vp, size] of Object.entries(VIEWPORTS)) {
	const page = await browser.newPage({ viewport: size, deviceScaleFactor: 1 });
	for (const [name, url] of Object.entries(PAGES)) {
		await page.goto(origin + url, { waitUntil: 'networkidle' });
		if (name === 'search') {
			if (vp === 'desktop') await page.keyboard.press('Control+k');
			else await page.click('site-search button[data-open-modal]');
			await page.waitForTimeout(300);
			await page.keyboard.type('cross');
			await page.waitForTimeout(1200);
		}
		const overflow = await page.evaluate(() => document.documentElement.scrollWidth - window.innerWidth);
		if (overflow > 0) problems.push(`${name}-${vp}: horizontal overflow ${overflow}px`);
		await page.screenshot({ path: `${out}/${name}-${vp}.png`, fullPage: name !== 'search' && name !== 'module' });
	}
	await page.close();
}
await browser.close();
server.close();
for (const p of problems) console.error(p);
console.log(`screenshot: wrote ${Object.keys(PAGES).length * 2} images to ${out}${problems.length ? `, ${problems.length} overflow problem(s)` : ', no horizontal overflow'}`);
process.exit(problems.length ? 1 : 0);
