#include "stdafx.h"

void Shadow::setupGL() {
	initializeOpenGLFunctions();

	/// shadow texture
    glGenFramebuffers(1, &shadowFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

    glGenTextures(1, &shadowTexture);
    glBindTexture(GL_TEXTURE_2D, shadowTexture);

    glTextureStorage2D(shadowTexture, 1, GL_R32F,
        Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT
    );

    static const float borderColourBlack[] = { 0.0f, 0.0f, 0.0f, 1.0f };

    glTextureParameteri(shadowTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(shadowTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(shadowTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(shadowTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameteri(shadowTexture, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTextureParameteri(shadowTexture, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
    glTextureParameterfv(shadowTexture, GL_TEXTURE_BORDER_COLOR, borderColourBlack);

    glNamedFramebufferTexture(
        shadowFramebuffer,
        GL_COLOR_ATTACHMENT0,
        shadowTexture,
        0
    );

    glGenRenderbuffers(1, &shadowRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, shadowRenderbuffer);
    glNamedRenderbufferStorage(
        shadowRenderbuffer,
        GL_DEPTH_COMPONENT32,
        Display::display.TEXTURE_WIDTH,
        Display::display.TEXTURE_HEIGHT
    );

    glNamedFramebufferRenderbuffer(
        shadowFramebuffer,
        GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER,
        shadowRenderbuffer
    );

    glNamedFramebufferDrawBuffer(shadowFramebuffer, GL_COLOR_ATTACHMENT0);
}

void Shadow::blurShadow(unsigned int rounds)
{
    switch (Lighting::lighting.shadowSettings.blurAlgorithm) {
    case GLConfiguration::BlurAlgorithm::Gaussian:
        GLConfiguration::configuration.gaussianRBlur->blur(shadowTexture, rounds);
        break;
    case GLConfiguration::BlurAlgorithm::Box:
        GLConfiguration::configuration.boxRBlur->blur(shadowTexture, rounds);
        break;
    case GLConfiguration::BlurAlgorithm::Kawase:
        GLConfiguration::configuration.kawaseRBlur->blur(shadowTexture, rounds);
        break;
    case GLConfiguration::BlurAlgorithm::Dual:
        GLConfiguration::configuration.dualRBlur->blur(shadowTexture, rounds);
        break;
    }
}
