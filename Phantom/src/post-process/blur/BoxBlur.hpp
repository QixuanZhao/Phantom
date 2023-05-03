#pragma once

template<>
inline BoxBlur<GL_RGB32F>::BoxBlur() : Blur(GLConfiguration::configuration.boxBlurRGBSp)
{ }

template<>
inline BoxBlur<GL_RG32F>::BoxBlur() : Blur(GLConfiguration::configuration.boxBlurRGSp)
{ }

template<>
inline BoxBlur<GL_R32F>::BoxBlur() : Blur(GLConfiguration::configuration.boxBlurRSp)
{ }

template<GLenum internalFormat>
inline BoxBlur<internalFormat>::~BoxBlur() { }
