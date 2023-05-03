#pragma once

#ifndef GL_CONFIGURATION_H
#define GL_CONFIGURATION_H

class ShaderProgram;
class GBuffer;
class SSAO;
class Bloom;

class GLConfiguration
{
public:
	static GLConfiguration configuration;

	bool updated = false;

	GLenum colourAttachments[32] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2,
		GL_COLOR_ATTACHMENT3,
		GL_COLOR_ATTACHMENT4,
		GL_COLOR_ATTACHMENT5,
		GL_COLOR_ATTACHMENT6,
		GL_COLOR_ATTACHMENT7,
		GL_COLOR_ATTACHMENT8,
		GL_COLOR_ATTACHMENT9,
		GL_COLOR_ATTACHMENT10,
		GL_COLOR_ATTACHMENT11,
		GL_COLOR_ATTACHMENT12,
		GL_COLOR_ATTACHMENT13,
		GL_COLOR_ATTACHMENT14,
		GL_COLOR_ATTACHMENT15,
		GL_COLOR_ATTACHMENT16,
		GL_COLOR_ATTACHMENT17,
		GL_COLOR_ATTACHMENT18,
		GL_COLOR_ATTACHMENT19,
		GL_COLOR_ATTACHMENT20,
		GL_COLOR_ATTACHMENT21,
		GL_COLOR_ATTACHMENT22,
		GL_COLOR_ATTACHMENT23,
		GL_COLOR_ATTACHMENT24,
		GL_COLOR_ATTACHMENT25,
		GL_COLOR_ATTACHMENT26,
		GL_COLOR_ATTACHMENT27,
		GL_COLOR_ATTACHMENT28,
		GL_COLOR_ATTACHMENT29,
		GL_COLOR_ATTACHMENT30,
		GL_COLOR_ATTACHMENT31
	};

	enum class BlurAlgorithm {
		Gaussian = 0,
		Box = 1,
		Kawase = 2,
		Dual = 3
	};

	ShaderProgram* sp = nullptr;
	ShaderProgram* orthographicShadowSp = nullptr;
	ShaderProgram* orthographicShadowInterpreterSp = nullptr;
	ShaderProgram* omnidirectionalShadowSp = nullptr;
	ShaderProgram* omnidirectionalShadowInterpreterSp = nullptr;
	ShaderProgram* lightSourceSp = nullptr;
	ShaderProgram* axisSp = nullptr;

	ShaderProgram* gaussianBlurRGBSp = nullptr;
	ShaderProgram* gaussianBlurRGSp = nullptr;
	ShaderProgram* gaussianBlurRSp = nullptr;

	ShaderProgram* boxBlurRGBSp = nullptr;
	ShaderProgram* boxBlurRGSp = nullptr;
	ShaderProgram* boxBlurRSp = nullptr;

	ShaderProgram* kawaseBlurRGBSp = nullptr;
	ShaderProgram* kawaseBlurRGSp = nullptr;
	ShaderProgram* kawaseBlurRSp = nullptr;

	ShaderProgram* dualDownBlurRGBSp = nullptr;
	ShaderProgram* dualDownBlurRGSp = nullptr;
	ShaderProgram* dualDownBlurRSp = nullptr;

	ShaderProgram* dualUpBlurRGBSp = nullptr;
	ShaderProgram* dualUpBlurRGSp = nullptr;
	ShaderProgram* dualUpBlurRSp = nullptr;

	ShaderProgram* copyRGBSp = nullptr;
	ShaderProgram* copyRGSp = nullptr;
	ShaderProgram* copyRSp = nullptr;

	ShaderProgram* hdrSp = nullptr;
	ShaderProgram* bloomDownSp = nullptr;
	ShaderProgram* bloomUpSp = nullptr;
	ShaderProgram* textureSp = nullptr;
	ShaderProgram* textureArraySp = nullptr;
	ShaderProgram* textureCubeSp = nullptr;
	ShaderProgram* textureCubeArraySp = nullptr;

	ShaderProgram* gpSp = nullptr;
	ShaderProgram* orthographicShadowInterpreterDeferredSp = nullptr;
	ShaderProgram* omnidirectionalShadowInterpreterDeferredSp = nullptr;
	ShaderProgram* lightingDeferredSp = nullptr;
	ShaderProgram* ssaoSp = nullptr;

	ShaderProgram* pbrSp = nullptr;

	bool deferredRendering = false;
	bool pbrEnabled = false;
	GBuffer* gBuffer = nullptr;
	SSAO* ssao = nullptr;

	TextureCopier<GL_RGB32F>* rgbCopier = nullptr;
	TextureCopier<GL_RG32F>* rgCopier = nullptr;
	TextureCopier<GL_R32F>* rCopier = nullptr;

	GaussianBlur<GL_RGB32F>* gaussianRGBBlur = nullptr;
	GaussianBlur<GL_R32F>* gaussianRBlur = nullptr;
	BoxBlur<GL_RGB32F>* boxRGBBlur = nullptr;
	BoxBlur<GL_R32F>* boxRBlur = nullptr;
	KawaseBlur<GL_RGB32F>* kawaseRGBBlur = nullptr;
	KawaseBlur<GL_R32F>* kawaseRBlur = nullptr;
	DualBlur<GL_RGB32F>* dualRGBBlur = nullptr;
	DualBlur<GL_R32F>* dualRBlur = nullptr;

	Bloom* bloom = nullptr;

	int textureCount = 0;

	void loadShaderProgram();
	void loadOrthographicalShadowShaderProgram();
	void loadOrthographicalShadowInterpreterShaderProgram();
	void loadOmnidirectionalShadowShaderProgram();
	void loadOmnidirectionalShadowInterpreterShaderProgram();
	void loadLightSourceShaderProgram();
	void loadAxisShaderProgram();

	void loadBlurShaderProgram();

	void loadCopyShaderProgram();

	void loadHDRShaderProgram();
	void loadBloomShaderProgram();
	void loadTextureShaderProgram();
	void loadTextureArrayShaderProgram();
	void loadTextureCubeShaderProgram();
	void loadTextureCubeArrayShaderProgram();

	void loadGeometryPassShaderProgram();
	void loadOrthographicShadowInterpreterDeferredShaderProgram();
	void loadOmnidirectionalShadowInterpreterDeferredShaderProgram();
	void loadLightingDeferredShaderProgram();
	void loadSSAOShaderProgram();

	void loadPBRShaderProgram();

	void initializeGBuffer();
	void initializeTextureCopier();
	void initializeBlur();
	void initializeBloom();

	inline void loadShaderPrograms() {
		loadShaderProgram();
		loadOrthographicalShadowShaderProgram(); 
		loadOrthographicalShadowInterpreterShaderProgram();
		loadOmnidirectionalShadowShaderProgram();
		loadOmnidirectionalShadowInterpreterShaderProgram();
		loadLightSourceShaderProgram();
		loadAxisShaderProgram();
		loadBlurShaderProgram();
		loadCopyShaderProgram();
		loadHDRShaderProgram();
		loadBloomShaderProgram();
		loadTextureShaderProgram();
		loadTextureArrayShaderProgram();
		loadTextureCubeShaderProgram();
		loadTextureCubeArrayShaderProgram();

		loadGeometryPassShaderProgram();
		loadOrthographicShadowInterpreterDeferredShaderProgram();
		loadOmnidirectionalShadowInterpreterDeferredShaderProgram();
		loadLightingDeferredShaderProgram();
		loadSSAOShaderProgram();

		loadPBRShaderProgram();
	}
};

#endif