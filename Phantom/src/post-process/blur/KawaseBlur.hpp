#pragma once

template<>
inline KawaseBlur<GL_RGB32F>::KawaseBlur() : 
    Blur(GLConfiguration::configuration.kawaseBlurRGBSp),
    copier(GLConfiguration::configuration.rgbCopier)
{
    GLF::glTextureParameteri(Blur<GL_RGB32F>::texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLF::glTextureParameteri(Blur<GL_RGB32F>::texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

template<>
inline KawaseBlur<GL_RG32F>::KawaseBlur() : 
    Blur(GLConfiguration::configuration.kawaseBlurRGSp),
    copier(GLConfiguration::configuration.rgCopier)
{
    GLF::glTextureParameteri(Blur<GL_RG32F>::texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLF::glTextureParameteri(Blur<GL_RG32F>::texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

template<>
inline KawaseBlur<GL_R32F>::KawaseBlur() : 
    Blur(GLConfiguration::configuration.kawaseBlurRSp),
    copier(GLConfiguration::configuration.rCopier)
{
    GLF::glTextureParameteri(Blur<GL_R32F>::texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLF::glTextureParameteri(Blur<GL_R32F>::texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
}

template<GLenum internalFormat>
inline KawaseBlur<internalFormat>::~KawaseBlur() { }

template<GLenum internalFormat>
inline void KawaseBlur<internalFormat>::blur(GLuint imageTexture, GLuint rounds)
{
    GLF::glTextureParameteri(imageTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GLF::glTextureParameteri(imageTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    GLF::glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
    GLF::glBindFramebuffer(GL_FRAMEBUFFER, Blur<internalFormat>::framebuffer);
    GLF::glDisable(GL_DEPTH_TEST);
    Blur<internalFormat>::sp->set("image", 0);
    GLF::glActiveTexture(GL_TEXTURE0);

    for (int i = 0; i < rounds; i++) {
        if (i % 2 == 1) {
            GLF::glBindTexture(GL_TEXTURE_2D, Blur<internalFormat>::texture);
            GLF::glNamedFramebufferTexture(Blur<internalFormat>::framebuffer, GL_COLOR_ATTACHMENT0, imageTexture, 0);
        }
        else {
            GLF::glBindTexture(GL_TEXTURE_2D, imageTexture);
            GLF::glNamedFramebufferTexture(Blur<internalFormat>::framebuffer, GL_COLOR_ATTACHMENT0, Blur<internalFormat>::texture, 0);
        }

        Blur<internalFormat>::sp->set("space", i);

        Display::screenQuad->draw(*Blur<internalFormat>::sp);
    }

    if (rounds % 2 == 1) {
        GLF::glBindTexture(GL_TEXTURE_2D, Blur<internalFormat>::texture);
        GLF::glNamedFramebufferTexture(Blur<internalFormat>::framebuffer, GL_COLOR_ATTACHMENT0, imageTexture, 0);
        // copy
        copier->copy(Blur<internalFormat>::texture, imageTexture);
    }

    GLF::glEnable(GL_DEPTH_TEST);
}
