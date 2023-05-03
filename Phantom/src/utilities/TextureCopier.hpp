#pragma once

template<>
inline TextureCopier<GL_RGB32F>::TextureCopier() : 
	sp(GLConfiguration::configuration.copyRGBSp)
{
	setupGL();
}

template<>
inline TextureCopier<GL_RG32F>::TextureCopier() :
	sp(GLConfiguration::configuration.copyRGSp)
{
	setupGL();
}

template<>
inline TextureCopier<GL_R32F>::TextureCopier() :
	sp(GLConfiguration::configuration.copyRSp)
{
	setupGL();
}

template<GLenum internalFormat>
inline TextureCopier<internalFormat>::~TextureCopier()
{
	glDeleteFramebuffers(1, &framebuffer);
}

template<GLenum internalFormat>
inline void TextureCopier<internalFormat>::setupGL()
{
	initializeOpenGLFunctions();

	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
}

template<GLenum internalFormat>
inline void TextureCopier<internalFormat>::copy(GLuint sourceTexture, GLuint targetTexture)
{
	glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, targetTexture, 0);
	sp->set("image", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, sourceTexture);
	glDisable(GL_DEPTH_TEST);
	Display::screenQuad->draw(*sp);
	glEnable(GL_DEPTH_TEST);
}
