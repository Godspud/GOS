import React from 'react';
import clsx from 'clsx';
import styles from './styles.module.css';

const FeatureList = [
	{
		title: 'Easy to Use',
		Svg: require('@site/static/img/undraw_docusaurus_mountain.svg').default,
		size: { width: 400, height: 400 },
		description: <>Docusaurus was designed to be easily installed.</>,
	},
	{
		title: '',
		image: require('@site/static/img/logo_512x512.png').default,
		size: { width: 512, height: 512 },
		description: <></>,
	},
	{
		title: 'Current time using CMOS',
		image: require('@site/static/img/time_screenshot.png').default,
		size: { width: 1600, height: 400 },
		description: <>Time is in the top Right corner</>,
	},
];

// 1. Update this function signature to include 'image' and 'size'
function Feature({ Svg, image, title, description, size }) {
	return (
		<div className={clsx('col col--4')}>
			<div className="text--center">
				{/* 2. Logic to switch between SVG and PNG */}
				{Svg ? (
					<Svg
						className={styles.featureSvg}
						role="img"
						style={{ width: size?.width, height: size?.height }}
					/>
				) : (
					<img
						src={image}
						className={styles.featureSvg}
						alt={title}
						style={{
							width: size?.width,
							height: size?.height,
							objectFit: 'contain',
						}}
					/>
				)}
			</div>
			<div className="text--center padding-horiz--md">
				<h3>{title}</h3>
				<p>{description}</p>
			</div>
		</div>
	);
}

export default function HomepageFeatures() {
	return (
		<section className={styles.features}>
			<div className="container">
				<div className="row">
					{/* This works because {...props} sends everything in FeatureList to Feature */}
					{FeatureList.map((props, idx) => (
						<Feature key={idx} {...props} />
					))}
				</div>
			</div>
		</section>
	);
}
