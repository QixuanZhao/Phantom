#include "DualBlur.h"
#pragma once


template<>
inline DualBlur<GL_RGB32F>::DualBlur() :
	Blur(GLConfiguration::configuration.dualDownBlurRGBSp),
	upSp(GLConfiguration::configuration.dualUpBlurRGBSp),
	copier(GLConfiguration::configuration.rgbCopier)
{
	setupGL();
}

template<>
inline DualBlur<GL_RG32F>::DualBlur() :
	Blur(GLConfiguration::configuration.dualDownBlurRGSp),
	upSp(GLConfiguration::configuration.dualUpBlurRGSp),
	copier(GLConfiguration::configuration.rgCopier)
{
	setupGL();
}

template<>
inline DualBlur<GL_R32F>::DualBlur() :
	Blur(GLConfiguration::configuration.dualDownBlurRSp),
	upSp(GLConfiguration::configuration.dualUpBlurRSp),
	copier(GLConfiguration::configuration.rCopier)
{
	setupGL();
}

template<GLenum internalFormat>
inline DualBlur<internalFormat>::~DualBlur()
{
	GLF::glDeleteTextures(9, textures + 1);
}

template<GLenum internalFormat>
inline void DualBlur<internalFormat>::blur(GLuint imageTexture, GLuint rounds)
{
	GLF::glBindFramebuffer(GL_FRAMEBUFFER, Blur<internalFormat>::framebuffer);
	GLF::glActiveTexture(GL_TEXTURE0);
	downSp->set("image", 0);
	upSp->set("source", 0);
	//upSp->set("radius", 1.0f * radius);
	textures[0] = imageTexture;

	constexpr int textureCount = sizeof(textures) / sizeof(GLuint);
	const int count = glm::min(int(rounds), textureCount);

	float width = Display::display.TEXTURE_WIDTH;
	float height = Display::display.TEXTURE_HEIGHT;
	for (int i = 1; i < count; i++) {
		width /= 2;
		height /= 2;
		GLF::glViewport(0, 0, width, height);
		GLF::glNamedFramebufferTexture(Blur<internalFormat>::framebuffer, GL_COLOR_ATTACHMENT0, textures[i], 0);
		GLF::glBindTexture(GL_TEXTURE_2D, textures[i - 1]);
		Display::screenQuad->draw(*downSp);
	}

	for (int i = count - 1; i > 0; i--) {
		width *= 2;
		height *= 2;
		GLF::glViewport(0, 0, width, height);
		GLF::glNamedFramebufferTexture(Blur<internalFormat>::framebuffer, GL_COLOR_ATTACHMENT0, textures[i - 1], 0);
		GLF::glBindTexture(GL_TEXTURE_2D, textures[i]);
		Display::screenQuad->draw(*upSp);
	}

	GLF::glEnable(GL_DEPTH_TEST);
}

template<GLenum internalFormat>
inline void DualBlur<internalFormat>::setupGL()
{
	textures[0] = Blur<internalFormat>::texture;
	GLF::glTextureParameteri(textures[0], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	GLF::glTextureParameteri(textures[0], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLF::glGenTextures(9, textures + 1);

	float width = Display::display.TEXTURE_WIDTH / 2.0f;
	float height = Display::display.TEXTURE_HEIGHT / 2.0f;
	for (int i = 1; i < 10; i++) {
		GLF::glBindTexture(GL_TEXTURE_2D, textures[i]);
		GLF::glTextureStorage2D(textures[i], 1, internalFormat,
			width, height);

		GLF::glTextureParameteri(textures[i], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		GLF::glTextureParameteri(textures[i], GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		GLF::glTextureParameteri(textures[i], GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		GLF::glTextureParameteri(textures[i], GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		width /= 2.0f;
		height /= 2.0f;
	}
}
