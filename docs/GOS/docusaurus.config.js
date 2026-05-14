// @ts-check
// `@type` JSDoc annotations allow editor autocompletion and type checking
// (when paired with `@ts-check`).
// There are various equivalent ways to declare your Docusaurus config.
// See: docusaurus.io

import { themes as prismThemes } from 'prism-react-renderer';

/**
 * Helper function to recursively sort sidebar items: alphabetical + folders on top
 * @param {any[]} items
 * @returns {any[]}
 */
function sortSidebarItems(items) {
	// 1. Sort everything alphabetically by label first
	items.sort((a, b) => {
		const labelA = ('label' in a ? a.label : 'id' in a ? a.id : '') || '';
		const labelB = ('label' in b ? b.label : 'id' in b ? b.id : '') || '';
		return labelA.localeCompare(labelB);
	});

	// 2. Sort categories (folders) above docs (files)
	items.sort((a, b) => {
		const aIsCat = a.type === 'category';
		const bIsCat = b.type === 'category';
		if (aIsCat && !bIsCat) return -1;
		if (!aIsCat && bIsCat) return 1;
		return 0;
	});

	// 3. Process nested categories recursively
	items.forEach((item) => {
		if (item.type === 'category' && 'items' in item && item.items) {
			sortSidebarItems(item.items);
		}
	});

	return items;
}

// This runs in Node.js - Don't use client-side code here (browser APIs, JSX...)

/** @type {import('@docusaurus/types').Config} */
const config = {
	title: 'GOS - wiki',
	tagline: 'GOS - wiki',
	favicon: 'img/logo.ico',

	// Future flags, see docusaurus.io
	future: {
		v4: true, // Improve compatibility with the upcoming Docusaurus v4
	},

	// Set the production url of your site here
	url: 'https://github.com',
	// Set the /<baseUrl>/ pathname under which your site is served
	// For GitHub pages deployment, it is often '/<projectName>/'
	baseUrl: '/godspud/GOS',

	// GitHub pages deployment config.
	// If you aren't using GitHub pages, you don't need these.
	organizationName: 'godspud', // Usually your GitHub org/user name.
	projectName: 'GOS', // Usually your repo name.

	onBrokenLinks: 'warn',

	// Even if you don't use internationalization, you can use this field to set
	// useful metadata like html lang. For example, if your site is Chinese, you
	// may want to replace "en" with "zh-Hans".
	i18n: {
		defaultLocale: 'en',
		locales: ['en'],
	},

	presets: [
		[
			'classic',
			/** @type {import('@docusaurus/preset-classic').Options} */
			({
				docs: {
					sidebarPath: './sidebars.js',
					// Please change this to your repo.
					// Remove this to remove the "edit this page" links.
					editUrl: 'github.com',
					// Custom generator intercepts sidebar generation
					sidebarItemsGenerator: async ({
						defaultSidebarItemsGenerator,
						...args
					}) => {
						const sidebarItems =
							await defaultSidebarItemsGenerator(args);
						return sortSidebarItems(sidebarItems);
					},
				},
				theme: {
					customCss: './src/css/custom.css',
				},
			}),
		],
	],

	themeConfig:
		/** @type {import('@docusaurus/preset-classic').ThemeConfig} */
		({
			// Replace with your project's social card
			image: 'img/docusaurus-social-card.jpg',
			colorMode: {
				respectPrefersColorScheme: true,
			},
			navbar: {
				title: 'GOS',
				logo: {
					alt: 'Logo',
					src: 'img/logo.png',
				},
				items: [
					{
						type: 'docSidebar',
						sidebarId: 'tutorialSidebar',
						position: 'left',
						label: 'DOCS',
					},
					{ to: '/blog', label: 'Blog', position: 'left' },
					{
						href: 'github.com',
						label: 'GitHub',
						position: 'right',
					},
				],
			},
			// footer: {
			// 	style: 'dark',
			// 	links: [
			// 		{
			// 			title: 'Docs',
			// 			items: [
			// 				{
			// 					label: 'DOCS',
			// 					to: '/docs/intro',
			// 				},
			// 			],
			// 		},
			// 		{
			// 			title: 'More',
			// 			items: [
			// 				{
			// 					label: 'Blog',
			// 					to: '/blog',
			// 				},
			// 				{
			// 					label: 'GitHub',
			// 					href: 'github.com',
			// 				},
			// 			],
			// 		},
			// 	],
			// 	copyright: `No Copyright © ${new Date().getFullYear()} God_spud, Built with Docusaurus.`,
			// },
			prism: {
				theme: prismThemes.github,
				darkTheme: prismThemes.dracula,
			},
		}),
};

export default config;
