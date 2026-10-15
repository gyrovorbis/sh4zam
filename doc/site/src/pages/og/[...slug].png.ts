import type { APIRoute, GetStaticPaths } from 'astro';
import { getCollection } from 'astro:content';
import { renderOg, ogKicker } from '../../lib/og';
import upstream from '../../data/upstream.json';

export const getStaticPaths: GetStaticPaths = async () => {
	const docs = await getCollection('docs');
	return docs.map((entry) => ({
		params: { slug: entry.id === 'index' || entry.id === '' ? 'index' : entry.id },
		props: { title: entry.data.title, id: entry.id },
	}));
};

export const GET: APIRoute = async ({ props }) => {
	const png = await renderOg(props.title, props.id ? ogKicker(props.id) : 'Docs', upstream.version);
	return new Response(png, { headers: { 'Content-Type': 'image/png' } });
};
