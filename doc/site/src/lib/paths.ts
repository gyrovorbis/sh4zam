// URL helpers shared by components, endpoints and scripts.

/** `/api/vector/shz_vec3_cross/` -> `/api/vector/shz_vec3_cross.md`, `/` -> `/index.md` (base-aware). */
export function markdownTwinPath(pathname: string, base = '/'): string {
	const b = base.endsWith('/') ? base : base + '/';
	let rest = pathname.startsWith(b) ? pathname.slice(b.length) : pathname.replace(/^\//, '');
	rest = rest.replace(/\/$/, '');
	return `${b}${rest || 'index'}.md`;
}
