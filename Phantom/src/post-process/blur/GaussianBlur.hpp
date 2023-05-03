#pragma once

template<>
inline GaussianBlur<GL_RGB32F>::GaussianBlur() : Blur(GLConfiguration::configuration.gaussianBlurRGBSp)
{ }

template<>
inline GaussianBlur<GL_RG32F>::GaussianBlur() : Blur(GLConfiguration::configuration.gaussianBlurRGSp)
{ }

template<>
inline GaussianBlur<GL_R32F>::GaussianBlur() : Blur(GLConfiguration::configuration.gaussianBlurRSp)
{ }

template<GLenum internalFormat>
inline GaussianBlur<internalFormat>::~GaussianBlur() { }