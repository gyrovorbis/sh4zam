// Prefixes root-relative links in Markdown content with Astro's `base`, so the same
// content works at sh4zam.com/ and under a GitHub Pages project path.
export default function rehypeBaseLinks({ base = '/' } = {}) {
	const prefix = base.replace(/\/$/, '');
	return (tree) => {
		if (!prefix) return;
		const visit = (node) => {
			if (node.type === 'element') {
				for (const prop of ['href', 'src']) {
					const v = node.properties?.[prop];
					if (typeof v === 'string' && v.startsWith('/') && !v.startsWith('//') && !v.startsWith(prefix + '/')) {
						node.properties[prop] = prefix + v;
					}
				}
			}
			// Raw HTML written inside MDX (the landing page) arrives as JSX nodes.
			if (node.type === 'mdxJsxFlowElement' || node.type === 'mdxJsxTextElement') {
				for (const a of node.attributes || []) {
					if ((a.name === 'href' || a.name === 'src') && typeof a.value === 'string' && a.value.startsWith('/') && !a.value.startsWith('//') && !a.value.startsWith(prefix + '/')) {
						a.value = prefix + a.value;
					}
				}
			}
			for (const child of node.children || []) visit(child);
		};
		visit(tree);
	};
}
