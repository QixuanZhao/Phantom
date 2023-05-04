#pragma once

#ifndef LIGHTING_H
#define LIGHTING_H

class Lighting {
public:
	static Lighting lighting;

	static Object3D* lightSourceSphere;

	AmbientLight ambientLight;

	//ShadowCastingPointLight pointLight;

	list<ParallelLight> parallelLights;
	list<ShadowCastingPointLight> pointLights;
	list<ShadowCastingSpotlight> spotlights;

	int pickedPointLightIndex = -1;
	int pickedSpotlightIndex = -1;

	bool bilateralReflective = true;
	bool bloom = true;

	bool ssaoEnabled = true;
	int ssaoFilterSize = 8;
	bool ssaoRadiusFollowingCamera = false;
	float ssaoRadius = 5.0f;
	int ssaoBlurPassCount = 2;
	GLConfiguration::BlurAlgorithm ssaoBlurAlgorithm = GLConfiguration::BlurAlgorithm::Box;

	struct ShadowSettings {
		bool enabled = true;
		bool linearFilterEnabled = false;
		bool samplingEnabled = true;
		Shadow::SamplingMethod samplingMethod = Shadow::SamplingMethod::PCF;
		bool samplingRounded = false;
		unsigned int filterSize = 3;
		bool samplingStratified = true;
		float samplingSpread = Shadow::DEPTH_MAP_WIDTH / 1.5;

		int blurPassCount = 2;
		GLConfiguration::BlurAlgorithm blurAlgorithm = GLConfiguration::BlurAlgorithm::Box;
	} shadowSettings;

	void updateShadow();
};
 
#endif