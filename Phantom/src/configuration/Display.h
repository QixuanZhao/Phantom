#pragma once

#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{
public:
	enum class ToneMappingMethod {
		Reinhard,
		Exposure
	};

	static Display display;

	static Surface* screenQuad;
	static Surface* horizon;
	static Surface* skybox;

	CoordinateSystem axes;

	vec3 backgroundColour;

	ObserverCamera observerCamera;
	FPSCamera fpsCamera;

	bool observerMode = true;
	bool catchMouse = false;

	const int TEXTURE_WIDTH = 1920;
	const int TEXTURE_HEIGHT = 1080;

	const int LOG2_WIDTH = glm::log2(TEXTURE_WIDTH);
	const int LOG2_HEIGHT = glm::log2(TEXTURE_HEIGHT);

	int currentViewportWidth = 0;
	int currentViewportHeight = 0;

	float gamma = 2.2f;
	
	int blurPassCount = 5;
	GLConfiguration::BlurAlgorithm blurAlgorithm = GLConfiguration::BlurAlgorithm::Kawase;

	bool toneMappingEnabled = true;
	float exposure = 1.0f;
	ToneMappingMethod toneMappingMethod = ToneMappingMethod::Exposure;

	inline MouseCamera& mouseCamera() {
		if (observerMode) return observerCamera;
		else return fpsCamera;
	}

	inline FlashCamera& flashCamera() {
		if (observerMode) return observerCamera;
		else return fpsCamera;
	}
};

#endif