#!/usr/bin/env node
// Favicons from src/assets/logo_small.png (a copy of doc/logo_small.png), nearest-neighbor scaled so the pixel art stays crisp.
// Run once after changing the logo: node scripts/gen-icons.mjs (outputs are committed).
import fs from 'node:fs';
import sharp from 'sharp';

const src = 'src/assets/logo_small.png';
const square = async (size) =>
	sharp(src)
		.resize(size, size, { fit: 'contain', kernel: 'nearest', background: { r: 0, g: 0, b: 0, alpha: 0 } })
		.png()
		.toBuffer();

const png32 = await square(32);
const png48 = await square(48);
fs.writeFileSync('public/favicon-32.png', png32);
fs.writeFileSync('public/apple-touch-icon.png', await sharp(await square(180)).flatten({ background: '#000000' }).png().toBuffer());

// ICO container holding PNG images (supported by every current browser).
const images = [png32, png48];
const header = Buffer.alloc(6 + 16 * images.length);
header.writeUInt16LE(0, 0);
header.writeUInt16LE(1, 2);
header.writeUInt16LE(images.length, 4);
let offset = header.length;
images.forEach((img, i) => {
	const size = i === 0 ? 32 : 48;
	const e = 6 + 16 * i;
	header.writeUInt8(size, e);
	header.writeUInt8(size, e + 1);
	header.writeUInt8(0, e + 2);
	header.writeUInt8(0, e + 3);
	header.writeUInt16LE(1, e + 4);
	header.writeUInt16LE(32, e + 6);
	header.writeUInt32LE(img.length, e + 8);
	header.writeUInt32LE(offset, e + 12);
	offset += img.length;
});
fs.writeFileSync('public/favicon.ico', Buffer.concat([header, ...images]));

const b64 = png48.toString('base64');
fs.writeFileSync(
	'public/favicon.svg',
	`<svg xmlns="http://www.w3.org/2000/svg" viewBox="0 0 48 48"><image width="48" height="48" style="image-rendering:pixelated" href="data:image/png;base64,${b64}"/></svg>\n`,
);
console.log('gen-icons: wrote public/favicon.{svg,ico}, favicon-32.png, apple-touch-icon.png');
