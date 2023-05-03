#include "stdafx.h"

Bloom::Bloom() : 
	downSp(GLConfiguration::configuration.bloomDownSp),
	upSp(GLConfiguration::configuration.bloomUpSp)
{
	initializeOpenGLFunctions();

	constexpr int count = sizeof(textures) / sizeof(GLuint);

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(count, textures);

	int width = Display::display.TEXTURE_WIDTH / 2;
	int height = Display::display.TEXTURE_HEIGHT / 2;
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	for (int i = 1; i < count; i++) {
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glTextureStorage2D(textures[i], 1, GL_RGBA32F, width, height);

		glTextureParameteri(textures[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTextureParameteri(textures[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTextureParameteri(textures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTextureParameteri(textures[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		width /= 2;
		height /= 2;
	}
}

Bloom::~Bloom()
{
	constexpr int count = sizeof(textures) / sizeof(GLuint);

	glDeleteTextures(count, textures);
	glDeleteFramebuffers(1, &framebuffer);
}

void Bloom::process(GLuint sourceTexture)
{
	glDisable(GL_DEPTH_TEST);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glActiveTexture(GL_TEXTURE0);
	downSp->set("source", 0);
	upSp->set("source", 0);
	upSp->set("target", 1);
	textures[0] = sourceTexture;

	constexpr int count = sizeof(textures) / sizeof(GLuint);

	float width = Display::display.TEXTURE_WIDTH;
	float height = Display::display.TEXTURE_HEIGHT;

	for (int i = 1; i < count; i++) {
		width /= 2;
		height /= 2;
		glViewport(0, 0, width, height);
		glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, textures[i], 0);
		glBindTexture(GL_TEXTURE_2D, textures[i - 1]);
		Display::screenQuad->draw(*downSp);
	}

	for (int i = count - 1; i > 0; i--) {
		width *= 2;
		height *= 2;
		glViewport(0, 0, width, height);
		glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, textures[i - 1], 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, textures[i]);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, textures[i - 1]);
		upSp->set("mixFactor", i == 1 ? 0.004 : 0.5);
		Display::screenQuad->draw(*upSp);
	}

	glEnable(GL_DEPTH_TEST);
}
