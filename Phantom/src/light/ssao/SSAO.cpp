#include "stdafx.h"

SSAO::SSAO() : 
	sp(GLConfiguration::configuration.ssaoSp), 
	gBuffer(GLConfiguration::configuration.gBuffer)
{
	initializeOpenGLFunctions();
	textureImageUnit = GLConfiguration::configuration.textureCount++;

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &texture);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTextureStorage2D(texture, 1, GL_R32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);

	static const float borderColourBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	//glTextureParameterfv(texture, GL_TEXTURE_BORDER_COLOR, borderColourBlack);

	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, texture, 0);
	glNamedFramebufferDrawBuffer(framebuffer, GL_COLOR_ATTACHMENT0);
}

SSAO::~SSAO()
{
	glDeleteTextures(1, &texture);
	glDeleteFramebuffers(1, &framebuffer);
}

void SSAO::render()
{
	glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gBuffer->viewSpacePositionTexture());

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gBuffer->viewSpaceNormalTexture());

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gBuffer->depthTexture());

	//Display::display.camera.configureShaderProgram(sp);
	sp->set("projection", Display::display.mouseCamera().getProjection());
	sp->set("input_position_view", 0);
	sp->set("input_normal_view", 1);
	sp->set("input_depth", 2);

	sp->set("kernelSize", Lighting::lighting.ssaoFilterSize);
	sp->set("autoRadius", Lighting::lighting.ssaoRadiusFollowingCamera);
	if (Lighting::lighting.ssaoRadiusFollowingCamera) {
		sp->set("cDistance", Display::display.observerCamera.getDistance());
	}
	else sp->set("radius", Lighting::lighting.ssaoRadius);

	Display::screenQuad->draw(*sp);
}

void SSAO::blur()
{
	switch (Lighting::lighting.ssaoBlurAlgorithm) {
	case GLConfiguration::BlurAlgorithm::Gaussian:
		GLConfiguration::configuration.gaussianRBlur->blur(texture, Lighting::lighting.ssaoBlurPassCount);
		break;
	case GLConfiguration::BlurAlgorithm::Box:
		GLConfiguration::configuration.boxRBlur->blur(texture, Lighting::lighting.ssaoBlurPassCount);
		break;
	case GLConfiguration::BlurAlgorithm::Kawase:
		GLConfiguration::configuration.kawaseRBlur->blur(texture, Lighting::lighting.ssaoBlurPassCount);
		break;
	case GLConfiguration::BlurAlgorithm::Dual:
		GLConfiguration::configuration.dualRBlur->blur(texture, Lighting::lighting.ssaoBlurPassCount);
		break;
	}
}
