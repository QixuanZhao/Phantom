#pragma once

template<GLenum internalFormat>
inline Blur<internalFormat>::Blur(ShaderProgram*& sp) : sp(sp)
{
	initializeOpenGLFunctions();

	glGenFramebuffers(1, &framebuffer);
	glGenTextures(1, &texture);

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTextureStorage2D(texture, 1, internalFormat,
		Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);

	glTextureParameteri(texture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

template<GLenum internalFormat>
inline Blur<internalFormat>::~Blur()
{
	glDeleteTextures(1, &texture);
	glDeleteFramebuffers(1, &framebuffer);
}

template<GLenum internalFormat>
inline void Blur<internalFormat>::blur(GLuint imageTexture, GLuint rounds)
{
    glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, Blur<internalFormat>::framebuffer);
    glDepthFunc(GL_ALWAYS);
    sp->set("image", 0);
    glActiveTexture(GL_TEXTURE0);
    bool horizontal = false;
    for (GLuint i = 0; i < rounds * 2; i++) {
        if (horizontal) {
            glBindTexture(GL_TEXTURE_2D, Blur<internalFormat>::texture);
            glNamedFramebufferTexture(Blur<internalFormat>::framebuffer, GL_COLOR_ATTACHMENT0, imageTexture, 0);
        }
        else {
            glBindTexture(GL_TEXTURE_2D, imageTexture);
            glNamedFramebufferTexture(Blur<internalFormat>::framebuffer, GL_COLOR_ATTACHMENT0, Blur<internalFormat>::texture, 0);
        }

        sp->set("horizontal", horizontal);

        Display::screenQuad->draw(*sp);

        horizontal = !horizontal;
    }
    glDepthFunc(GL_LEQUAL);
}
