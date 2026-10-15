// Open Graph card renderer: black background, logo_small, page title in Roboto,
// one accent rule. 1200x630 PNG via satori + resvg.
import fs from 'node:fs';
import path from 'node:path';
import satori from 'satori';
import { Resvg } from '@resvg/resvg-js';

const root = process.cwd();
const font = (w: number) => fs.readFileSync(path.join(root, `node_modules/@fontsource/roboto/files/roboto-latin-${w}-normal.woff`));
const mono = fs.readFileSync(path.join(root, 'node_modules/@fontsource/jetbrains-mono/files/jetbrains-mono-latin-400-normal.woff'));
const logo = `data:image/png;base64,${fs.readFileSync(path.join(root, 'src/assets/logo_small.png')).toString('base64')}`;
const fonts = [
	{ name: 'Roboto', data: font(400), weight: 400 as const, style: 'normal' as const },
	{ name: 'Roboto', data: font(700), weight: 700 as const, style: 'normal' as const },
	{ name: 'JetBrains Mono', data: mono, weight: 400 as const, style: 'normal' as const },
];

const h = (type: string, style: Record<string, unknown>, children?: unknown) => ({ type, props: { style, children } });

export async function renderOg(title: string, kicker: string, version: string): Promise<Uint8Array> {
	const size = title.length > 34 ? 60 : 76;
	const tree = h(
		'div',
		{ width: '1200px', height: '630px', display: 'flex', flexDirection: 'column', justifyContent: 'space-between', background: '#000000', padding: '64px 72px', fontFamily: 'Roboto' },
		[
			h('div', { display: 'flex', alignItems: 'center', gap: '24px' }, [
				{ type: 'img', props: { src: logo, width: 128, height: 94, style: { imageRendering: 'pixelated' } } },
				h('div', { display: 'flex', flexDirection: 'column' }, [
					h('div', { fontSize: '44px', fontWeight: 700, color: '#D3ECE0' }, 'SH4ZAM!'),
					h('div', { fontSize: '24px', color: '#A7D8C0', fontFamily: 'JetBrains Mono' }, `v${version}`),
				]),
			]),
			h('div', { display: 'flex', flexDirection: 'column', gap: '20px' }, [
				h('div', { fontSize: '26px', color: '#7AC5A0', fontFamily: 'JetBrains Mono' }, kicker),
				h('div', { fontSize: `${size}px`, fontWeight: 700, color: '#D3ECE0', lineHeight: 1.1, wordBreak: 'break-all' }, title),
				h('div', { width: '160px', height: '6px', background: '#39C96B' }, ''),
			]),
			h('div', { fontSize: '24px', color: '#C9D1D9' }, "Fast math library for the Sega Dreamcast's SH4 CPU"),
		],
	);
	const svg = await satori(tree as never, { width: 1200, height: 630, fonts });
	return new Resvg(svg, { fitTo: { mode: 'width', value: 1200 } }).render().asPng();
}

export function ogKicker(id: string): string {
	const parts = id.split('/');
	if (parts[0] === 'api') {
		if (parts.length === 1) return 'API reference';
		if (parts[1] === 'cpp') return 'C++ API';
		return parts.length > 2 ? `API · ${parts[1]}` : 'API module';
	}
	if (parts[0] === 'guides') return 'Guide';
	if (parts[0] === 'concepts') return 'Concept';
	return 'Docs';
}
