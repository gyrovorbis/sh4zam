// Where the SH4ZAM repo hosting this site lives on GitHub, worked out at build time so
// forks link to themselves: GitHub Actions sets GITHUB_REPOSITORY; locally we read the
// `origin` remote. Override with DOCS_REPO=owner/name. Returns null when none of these are available.
import { execFileSync } from 'node:child_process';

let cached;

function docsRepo() {
	if (cached !== undefined) return cached;
	let repo = process.env.DOCS_REPO || process.env.GITHUB_REPOSITORY || '';
	if (!repo) {
		try {
			const url = execFileSync('git', ['remote', 'get-url', 'origin'], { encoding: 'utf8', stdio: ['ignore', 'pipe', 'ignore'] }).trim();
			const m = url.match(/github\.com[:/]([^/]+\/[^/]+?)(?:\.git)?$/);
			if (m) repo = m[1];
		} catch {}
	}
	cached = repo || null;
	return cached;
}

// The site's directory inside the repo.
const SITE_DIR = 'doc/site';

function docsRepoUrl() {
	const repo = docsRepo();
	return repo ? `https://github.com/${repo}` : null;
}

function docsBranch() {
	return process.env.DOCS_BRANCH || 'master';
}

/** Base URL for Starlight's "Edit page" links, or null. */
export function docsEditUrl() {
	const url = docsRepoUrl();
	return url ? `${url}/edit/${docsBranch()}/${SITE_DIR}/` : null;
}

/** The site's source directory on GitHub, or null. */
export function docsSourceUrl() {
	const url = docsRepoUrl();
	return url ? `${url}/tree/${docsBranch()}/${SITE_DIR}` : null;
}
