#pragma once

class ShaderProgram;

template<GLenum internalFormat>
class BoxBlur : public Blur<internalFormat> {
public:
	BoxBlur();
	virtual ~BoxBlur();
};
