#pragma once

#ifndef SHADOW_H
#define SHADOW_H
class Shadow : protected GLF {
public:
	enum class SamplingMethod {
		PCF = 0,
		PoissonDisk = 1
	};

	/// shadow map width <br/> 阴影图宽度
	static const unsigned int DEPTH_MAP_WIDTH = 2048;
	static const unsigned int DEPTH_MAP_HEIGHT = DEPTH_MAP_WIDTH;

	inline Shadow() { }
	inline Shadow(const Shadow& shadow) { }

	inline GLuint getShadowFramebufferName() const { return shadowFramebuffer; }
	inline GLuint getShadowTexture() const { return shadowTexture; }

	inline GLuint getDepthFramebufferName() const { return depthFramebuffer; }
	inline GLuint getDepthTexture() const { return depthTexture; }

	inline bool isUpdated() const { return updated; }
	inline void requireUpdate() { updated = false; }

	inline bool isEnabled() const { return enabled; }
	inline void setEnabled(bool enabled) { this->enabled = enabled; }

	inline bool isLinearFilterEnabled() const { return linearFilterEnabled; }
	inline void setLinearFilterEnabled(bool enabled) { this->linearFilterEnabled = enabled; }

	inline bool isSamplingEnabled() const { return samplingEnabled; }
	inline void setSamplingEnabled(bool enabled) { samplingEnabled = enabled; }

	inline SamplingMethod getSamplingMethod() const { return samplingMethod; }
	inline void setSamplingMethod(const SamplingMethod& samplingMethod) { this->samplingMethod = samplingMethod; }

	inline bool isSamplingRounded() const { return samplingRounded; }
	inline void setSamplingRounded(bool rounded) { samplingRounded = rounded; }

	inline unsigned getSamplingSize() const { return samplingSize; }
	inline void setSamplingSize(unsigned size) { samplingSize = size; }

	inline bool isSamplingStratified() const { return samplingStratified; }
	inline void setSamplingStratified(bool stratified) { samplingStratified = stratified; }

	inline float getSamplingSpread() const { return samplingSpread; }
	inline void setSamplingSpread(float spread) { samplingSpread = spread; }
	inline void adjustSamplingSpread() { samplingSpread = DEPTH_MAP_WIDTH / 1.5; }

	virtual void setupGL() = 0;
	virtual void updateDepth(ShaderProgram* sp) = 0;
	virtual void updateShadow(ShaderProgram* sp) = 0;
	virtual void blurShadow(unsigned int rounds = 2);

protected:
	inline void updateShadow() { updated = true; };

private:
	bool updated = false;

protected:
	bool ready = false;
	bool enabled = true;

	GLuint shadowFramebuffer = 0;
	GLuint shadowRenderbuffer = 0;
	GLuint shadowTexture = 0;

	GLuint depthFramebuffer = 0;
	GLuint depthTexture = 0;

	bool linearFilterEnabled = false;

	bool samplingEnabled = true;

	SamplingMethod samplingMethod = SamplingMethod::PoissonDisk;

	/// PCF options
	bool samplingRounded = false;
	unsigned samplingSize = 3;

	/// poisson disk options
	bool samplingStratified = true;
	float samplingSpread = DEPTH_MAP_WIDTH / 1.5;
};

#endif