#include "stdafx.h"

GBuffer::GBuffer() :
	sp(GLConfiguration::configuration.gpSp)
{
	initializeOpenGLFunctions();

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(count, textures);

	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	for (int i = 0; i < count; i++) glBindTexture(GL_TEXTURE_2D, textures[i]);

	glTextureStorage2D(textures[0], 1, GL_RGB32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glTextureStorage2D(textures[1], 1, GL_RGB32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glTextureStorage2D(textures[2], 1, GL_RGBA32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glTextureStorage2D(textures[3], 1, GL_RGB32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glTextureStorage2D(textures[4], 1, GL_RGB32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glTextureStorage2D(textures[5], 1, GL_RG32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glTextureStorage2D(textures[6], 1, GL_RGB32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glTextureStorage2D(textures[count - 1], 1, GL_DEPTH_COMPONENT32F, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);

	//static const float borderColourBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	for (int i = 0; i < count; i++) {
		glTextureParameteri(textures[i], GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTextureParameteri(textures[i], GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTextureParameteri(textures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textures[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textures[i], GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
		glTextureParameteri(textures[i], GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
		//glTextureParameterfv(textures[i], GL_TEXTURE_BORDER_COLOR, borderColourBlack);
		textureImageUnits[i] = GLConfiguration::configuration.textureCount++;
	}

	for (int i = 0; i < count - 1; i++)
		glNamedFramebufferTexture(framebuffer, GLConfiguration::configuration.colourAttachments[i], textures[i], 0);
	
	glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, textures[count - 1], 0);

	glNamedFramebufferDrawBuffers(framebuffer, count - 1, GLConfiguration::configuration.colourAttachments);
}

GBuffer::~GBuffer()
{
	glDeleteTextures(5, textures);
	glDeleteFramebuffers(1, &framebuffer);
}

void GBuffer::render()
{
	glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glNamedFramebufferDrawBuffer(framebuffer, GLConfiguration::configuration.colourAttachments[2]);
	glClearColor(
		Display::display.backgroundColour.r,
		Display::display.backgroundColour.g,
		Display::display.backgroundColour.b,
		1.0f
	);
	glClear(GL_COLOR_BUFFER_BIT);

	glNamedFramebufferDrawBuffers(framebuffer, count - 1, GLConfiguration::configuration.colourAttachments);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	//glDisable(GL_CULL_FACE);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	Display::display.mouseCamera().configureShaderProgram(sp);
	//if (Lighting::lighting.parallelLights.size() > 0) {
	//	sp->set("lightPV", Lighting::lighting.parallelLights.front().getShadow().getPV());
	//}
	if (Lighting::lighting.spotlights.size() > 0) {
		sp->set("lightPV", Lighting::lighting.spotlights.front().getShadow().getPV());
	}

	for (Object3D* object : Data::instance().getObjects()) {
		float theta = 0.01f * Control::instance().deltaTime;
		float cosineTheta = glm::cos(glm::radians(theta));
		float sineTheta = glm::sin(glm::radians(theta));
		vec3 axis = glm::normalize(vec3(0.0f, 0.0f, 1.0f));
		object->applyRotation(quat(cosineTheta, sineTheta * axis));
		object->draw(*sp);
	}

	Lighting::lighting.updateShadow();

	Display::horizon->setColour(glm::mix(Display::display.backgroundColour, vec3(0.5f), 0.1));
	float zoomDistance = Data::instance().getZoomDistance();
	if (zoomDistance < 1e-6f) zoomDistance = Display::display.observerCamera.FAR_PLANE_DISTANCE;
	Display::horizon->setScale(vec3(zoomDistance));
	Display::horizon->draw(*sp);
}
