#pragma once

class ShaderProgram;

template<GLenum internalFormat>
class KawaseBlur : public Blur<internalFormat> {
public:
	KawaseBlur();
	virtual ~KawaseBlur();

	virtual void blur(GLuint imageTexture, GLuint rounds) override;

protected:
	TextureCopier<internalFormat>*& copier;
};
