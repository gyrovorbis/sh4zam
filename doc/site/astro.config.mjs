// @ts-check
import fs from 'node:fs';
import { defineConfig } from 'astro/config';
import starlight from '@astrojs/starlight';
import sitemap from '@astrojs/sitemap';
import { lastmodMap } from './src/lib/lastmod.mjs';
import { docsEditUrl } from './src/lib/repo.mjs';
import rehypeBaseLinks from './src/plugins/rehype-base-links.mjs';

// SITE_URL may include a path (GitHub Pages preview: https://<user>.github.io/sh4zam).
const siteUrl = new URL(process.env.SITE_URL || 'https://sh4zam.com');
const base = siteUrl.pathname.replace(/\/$/, '') || '/';

const modules = ['scalar', 'trig', 'vector', 'quat', 'matrix', 'xmtrx', 'complex', 'memory', 'cdefs', 'version'];
const lastmod = lastmodMap(process.cwd());
const editUrl = docsEditUrl();
// Endpoints (Markdown twins, OG images, JSON) are not pages; keep them out of the sitemap.
const NON_PAGE = new RegExp('[.](md|png|json|txt)$');
const codeTheme = JSON.parse(fs.readFileSync(new URL('./src/styles/code-theme.json', import.meta.url), 'utf8'));

export default defineConfig({
	site: siteUrl.origin,
	base,
	trailingSlash: 'always',
	build: { format: 'directory' },
	markdown: {
		// Keep `--` and quotes exactly as written in the upstream headers.
		smartypants: false,
		rehypePlugins: [[rehypeBaseLinks, { base }]],
	},
	integrations: [
		sitemap({
			filter: (page) => !/\.(md|png|json|txt)$/.test(page),
			serialize(item) {
				const p = new URL(item.url).pathname.slice(base === '/' ? 0 : base.length);
				const d = lastmod.get(p);
				if (d) item.lastmod = d;
				return item;
			},
		}),
		starlight({
			title: 'SH4ZAM!',
			description: "Fast math library for the Sega Dreamcast's SH4 CPU.",
			defaultLocale: 'root',
			locales: { root: { label: 'English', lang: 'en' } },
			favicon: '/favicon.svg',
			lastUpdated: true,
			// No owner hard-coded: the repo comes from GitHub Actions or the git remote (src/lib/repo.mjs).
			...(editUrl ? { editLink: { baseUrl: editUrl } } : {}),
			social: [
				{ icon: 'github', label: 'GitHub', href: 'https://github.com/gyrovorbis/sh4zam' },
				{ icon: 'discord', label: 'Discord', href: 'https://discord.gg/G2Ay9kxec2' },
			],
			customCss: [
				'@fontsource/roboto/latin-400.css',
				'@fontsource/roboto/latin-500.css',
				'@fontsource/roboto/latin-700.css',
				'@fontsource/jetbrains-mono/latin-400.css',
				'@fontsource/jetbrains-mono/latin-700.css',
				'./src/styles/theme.css',
			],
			expressiveCode: {
				themes: [codeTheme],
				useStarlightDarkModeSwitch: false,
				useStarlightUiThemeColors: false,
				// Plain blocks: no editor tabs or terminal window chrome.
				defaultProps: { frame: 'none' },
				styleOverrides: {
					borderRadius: '4px',
					borderColor: '#30363D',
					codeFontFamily: "'JetBrains Mono', Consolas, Monaco, monospace",
					codeFontSize: '0.84rem',
					uiFontFamily: "'Roboto', sans-serif",
					frames: { shadowColor: 'transparent', frameBoxShadowCssValue: 'none' },
				},
			},
			components: {
				Header: './src/components/Header.astro',
				SiteTitle: './src/components/SiteTitle.astro',
				ThemeSelect: './src/components/ThemeSelect.astro',
				ThemeProvider: './src/components/ThemeProvider.astro',
				Hero: './src/components/Hero.astro',
				Footer: './src/components/Footer.astro',
				PageTitle: './src/components/PageTitle.astro',
				Head: './src/components/Head.astro',
			},
			sidebar: [
				{
					label: 'Start',
					items: ['guides/getting-started', 'guides/install-kallistios', 'guides/install-cmake', 'guides/using-in-a-project'],
				},
				{
					label: 'Guides',
					items: ['guides/interop', 'guides/matrix-transforms', 'guides/optimization', 'guides/examples', 'guides/testing-and-contributing'],
				},
				{
					label: 'Concepts',
					items: [
						'concepts/architecture',
						'concepts/naming-and-suffixes',
						'concepts/types-and-layout',
						'concepts/conventions',
						'concepts/xmtrx',
						'concepts/sh4-fpu',
						'concepts/c-and-cpp',
						'concepts/backends',
					],
				},
				{
					label: 'API',
					items: ['api', ...modules.map((m) => `api/${m}`), { label: 'C++', autogenerate: { directory: 'api/cpp' }, collapsed: true }, 'cheatsheet'],
				},
				{
					label: 'More',
					items: ['showcase', 'resources', 'community', 'contributing', 'changelog', 'for-agents'],
				},
			],
		}),
	],
});
