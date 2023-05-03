#pragma once

class ShaderProgram;

template<GLenum internalFormat>
class GaussianBlur : public Blur<internalFormat> {
public:
	GaussianBlur();
	virtual ~GaussianBlur();
};
