# SH4ZAM documentation site

A static site (Astro + Starlight) for sh4zam.com. It keeps the black-and-green look of the Doxygen site and adds things for both people and machines: guides and concept pages, one page per public symbol, a cheatsheet, a Markdown twin of every page, `llms.txt`, a JSON symbol index, clean URLs, and redirects from the old Doxygen URLs.

The Doxygen HTML (`make docs`, `doc/Doxyfile`) is untouched and still works.

## Two kinds of pages

| Kind | Where | Source |
| --- | --- | --- |
| **Generated**: symbol pages, module pages, C++ class pages, cheatsheet, `public/api/*.json`, `public/_redirects` | `src/content/docs/api/**`, `src/content/docs/cheatsheet.md`, `public/api/`, `public/_redirects`, `src/data/upstream.json` | `scripts/gen-api.mjs`, from the header comments in `include/sh4zam/` via Doxygen XML. Not committed; regenerated on every `npm run dev` and `npm run build`. To change them, edit the header comment. |
| **Handwritten**: landing page, guides, concepts, for-agents, showcase, etc. | everything else in `src/content/docs/` | Plain Markdown. Edit freely. |

The site documents whatever commit it is built from; the footer says which.

## Quick look

To just see the site, install Node.js 20+ and Doxygen (see Setup below) and run the script for your OS. It checks both, runs `npm ci` the first time (and again when `package-lock.json` changes), then opens a live preview at <http://localhost:4321/>.

- **Windows:** double-click `doc\site\run.cmd`, or run `.\run.cmd` from `doc\site`. Use the `.\`: nvm for Windows ships its own `run.cmd` on the `PATH`, and PowerShell will pick that one otherwise.
- **macOS / Linux:** `./doc/site/run.sh`

Add `--build` (`.\run.cmd --build`, `./run.sh --build`) for a full production build served from `dist/`, which includes `llms.txt` and the Markdown versions of each page. It takes about two minutes.

## Setup

1. **Node.js 20+** (CI-tested on 24): <https://nodejs.org/>
2. **Doxygen** (1.18.0 is what the output was checked against): `brew install doxygen`, `winget install DimitriVanHeesch.Doxygen`, or a release from <https://github.com/doxygen/doxygen/releases>. If it isn't on your `PATH`, set `DOXYGEN=/path/to/doxygen`.
3. Install:

```sh
cd doc/site
npm ci
```

## Commands

Run from `doc/site/`.

| Command | What it does |
| --- | --- |
| `npm run dev` | Regenerates the API pages, then serves a live preview at <http://localhost:4321/>. |
| `npm run build` | Regenerates the API pages, then builds into `dist/` (about 2 minutes; most of it is OG images) and writes `llms.txt`, `robots.txt` and the legacy Doxygen redirect pages. |
| `npm run check` | Verifies the built site: every page has a Markdown twin, `llms.txt` links resolve, `index.json` matches its schema and the headers, head tags, robots/sitemap, banned phrases, internal links, and every `shz_`/`SHZ_`/`shz::` name in handwritten pages exists. |
| `npm run gen` | Only regenerates the API pages. |
| `node scripts/run.mjs` | What `run.cmd` and `run.sh` call: checks Node and Doxygen, installs packages when needed, then `npm run dev` (or a build plus a static server with `--build`). |
| `npm run shots` | Screenshots of key pages at desktop and mobile size into `build/shots/`. |
| `node scripts/qa.mjs` | Accessibility (axe) and Lighthouse audits into `build/qa/`. |
| `node scripts/check-ai.mjs --external` | Also checks external links (warnings only). |

The generator prints a summary such as `800 symbols ... undocumented: 10, unresolved_refs: 7` and writes two reports:

- `build/undocumented.txt`: public symbols with no Doxygen comment. Their pages say "No description in the upstream header."
- `build/unresolved-refs.txt`: names mentioned in comments (`\sa foo()`, `foo()` in prose) that don't exist. They render as plain code instead of links.

## When the generator needs a small edit

All of these live in `scripts/data/modules.mjs`, which is plain data:

- **A new header/module** (say `shz_noise.h`): add an entry to `MODULES` (id, title, file names, a one-line `brief`, a few `intro` sentences, which `suffixes` apply), and add `'noise'` to the `modules` list in `astro.config.mjs` so it shows in the sidebar.
- **A new C++ class** (the generator prints `C++ class shz::foo has no entry in CPP_CLASSES; skipped`): add it to `CPP_CLASSES` with the C type it wraps.
- **A new suffix**: add it to `SUFFIXES` and to the `suffixes` list of the modules that use it.

## Editing the handwritten pages

- Files: `src/content/docs/guides/*.md`, `src/content/docs/concepts/*.md`, `src/content/docs/*.md`, and the landing page `src/content/docs/index.mdx`.
- Each file starts with frontmatter:

  ```markdown
  ---
  title: Matrix Transforms
  description: One sentence, 160 characters max, shown in search results and link previews
  ---
  ```

- Plain Markdown only (the landing page is MDX). Use `## C` / `## C++` headings rather than tabs, so the Markdown twins stay readable.
- Link to other pages with root paths like `/api/xmtrx/` or `/api/vector/shz_vec3_cross/`; the build fixes them up for the hosting path.
- **Sidebar order** is in `astro.config.mjs` (`sidebar:`). New handwritten pages must be added there.
- `npm run check` enforces the house rules: no em dashes, no emoji, no marketing words ("seamless", "robust", "powerful", "blazing fast", "modern", ...), and no `shz_` names that don't exist. A page that mentions a nonexistent name on purpose (e.g. `shz_memset`, in "things that do not exist") needs it added to `ALLOW` in `scripts/check-identifiers.mjs`.
- When `doc/tips.dox` gains text for its empty headings, copy it into `src/content/docs/guides/optimization.md`, which lists them as "not covered yet".

## Deploying

The build output (`dist/`) is a folder of static files. The site URL defaults to `https://sh4zam.com`; set `SITE_URL` to build for another host or a path prefix.

### GitHub Pages (the default)

`.github/workflows/docs-site.yml` builds and checks the site on every pull request that touches `doc/` or `include/`, and on pushes to `master` it also deploys to GitHub Pages. One-time setup: Settings → Pages → Build and deployment → Source: **GitHub Actions**. The site then lives at `https://<owner>.github.io/sh4zam/`; the workflow reads that URL from the Pages settings, so links, `llms.txt` and the JSON index all use it.

To serve it from a subdomain such as `docs.sh4zam.com`: Settings → Pages → Custom domain, add a `CNAME` record pointing that name at `<owner>.github.io`, tick "Enforce HTTPS" once the certificate is issued, and re-run the workflow.

### What updates on its own

Any push to `master` that touches `doc/` or `include/` rebuilds and redeploys the site. New functions, types and macros in existing headers, and changes to their comments, show up automatically.

Two things need a line in `scripts/data/modules.mjs` (see "When the generator needs a small edit" above):

- A **new header**: the generator warns `shz_foo.h has no entry in MODULES` and `npm run check` fails, so the workflow goes red and nothing deploys until it's added.
- A **new C++ class**: the generator warns `has no entry in CPP_CLASSES` and leaves it out.

Handwritten guides only change when someone edits them.

### FTP to sh4zam.com instead

```sh
cd doc/site
npm ci
npm run build
npm run check
```

Upload the **contents** of `dist/` to the web root, replacing the old Doxygen files.

- `dist/.htaccess` (from `public/.htaccess`) makes Apache serve `.md` as `text/markdown` and `.txt` as UTF-8. On nginx, add `types { text/markdown md; }`.
- Old Doxygen URLs keep working: `dist/` contains small redirect pages for `group__*.html`, `shz__*_8h.html`, `struct*.html`, etc., including `#anchor` deep links to individual functions. `public/_redirects` does the same for Netlify/Cloudflare Pages.

### Edit links

"Edit page" and "Site source" links point at `doc/site/` on the `master` branch of the repo the site is built from: `GITHUB_REPOSITORY` in GitHub Actions, otherwise the `origin` remote. Override with `DOCS_REPO=owner/name` and `DOCS_BRANCH=branch`.

## Troubleshooting

| Symptom | Fix |
| --- | --- |
| `doxygen failed to start` | Install Doxygen or set `DOXYGEN=/path/to/doxygen`. |
| `npm run check` reports `banned phrase` | Reword the sentence; the list is in `scripts/check-ai.mjs`. |
| `npm run check` reports `unknown C identifier` | Typo in a handwritten page, or the symbol was renamed. Fix the page. |
| `npm run check` reports a broken link | The page path in a Markdown link is wrong, or the target page was removed. |
| Build warns `Duplicate id ... found` | Stale Astro cache. Delete `.astro/` and `node_modules/.astro/` and rebuild. |

## Layout

```
astro.config.mjs       site config, sidebar, theme wiring
src/content/docs/      all pages (api/** and cheatsheet.md are generated, not committed)
src/components/        header, title, hero, footer, head overrides
src/styles/            theme.css (colors from the Doxygen site), code-theme.json
src/pages/             Markdown twin endpoint ([...slug].md.ts), OG image endpoint
scripts/gen-api.mjs    the API generator; scripts/data/modules.mjs is its hand-written data
scripts/Doxyfile.xml   XML-only Doxygen config layered on doc/Doxyfile
scripts/gen-llms.mjs   post-build: llms.txt, robots.txt, Doxygen redirect stubs
scripts/check-ai.mjs   post-build checks (npm run check)
scripts/run.mjs        one-step local preview behind run.cmd / run.sh
```
