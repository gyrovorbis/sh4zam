import type { APIRoute, GetStaticPaths } from 'astro';
import { getCollection } from 'astro:content';
import { renderTwin } from '../lib/twin';

export const getStaticPaths: GetStaticPaths = async () => {
	const docs = await getCollection('docs');
	return docs.map((entry) => ({
		params: { slug: entry.id === '' ? 'index' : entry.id },
		props: { entry },
	}));
};

export const GET: APIRoute = ({ props, site }) => {
	const { entry } = props;
	const base = import.meta.env.BASE_URL.replace(/\/$/, '');
	const siteBase = `${site!.origin}${base}`;
	const path = entry.id === 'index' || entry.id === '' ? '/' : `/${entry.id}/`;
	const body = renderTwin({
		title: entry.data.title,
		description: entry.data.description,
		body: entry.body ?? '',
		canonical: `${siteBase}${path}`,
		siteBase,
	});
	return new Response(body, { headers: { 'Content-Type': 'text/markdown; charset=utf-8' } });
};
