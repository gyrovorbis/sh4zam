// Markdown twin rendering: the page's source Markdown, frontmatter
// stripped, with a `# title` / `Source:` header and every root-relative link made absolute.

export interface TwinInput {
	title: string;
	description?: string;
	body: string;
	canonical: string; // absolute URL of the HTML page
	siteBase: string; // e.g. https://sh4zam.com or https://user.github.io/sh4zam
}

export function renderTwin({ title, description, body, canonical, siteBase }: TwinInput): string {
	let md = body
		// Generated-file notices and other HTML comments.
		.replace(/<!--[\s\S]*?-->\n?/g, '')
		// Layout wrappers the generator / landing page use for styling only.
		.replace(/^<\/?div[^>]*>\s*$/gm, '')
		.replace(/<span id="[^"]*"><\/span>/g, '');

	// Landing-page definition list -> Markdown list.
	md = md
		.replace(/<dl[^>]*>\s*/g, '')
		.replace(/<\/dl>\s*/g, '')
		.replace(/<dt><a href="([^"]+)">([^<]+)<\/a><\/dt>\s*<dd>([\s\S]*?)<\/dd>/g, (_, href, name, dd) => `- [${name}](${href}): ${htmlInline(dd)}`);

	// Root-relative links and images -> absolute.
	md = md.replace(/\]\((\/[^)\s]*)\)/g, (_, p) => `](${siteBase}${p})`);

	md = md.replace(/\n{3,}/g, '\n\n').trim();
	const head = [`# ${title}`, '', `Source: ${canonical}`];
	if (description && !md.includes(description)) head.push('', `> ${description}`);
	return `${head.join('\n')}\n\n${md}\n`;
}

function htmlInline(s: string): string {
	return s
		.replace(/<code>([^<]*)<\/code>/g, '`$1`')
		.replace(/&lt;/g, '<')
		.replace(/&gt;/g, '>')
		.replace(/&amp;/g, '&')
		.trim();
}
