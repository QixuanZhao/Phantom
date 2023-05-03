#pragma once

class ShaderProgram;

template<GLenum internalFormat>
class TextureCopier : protected GLF {
public:
	TextureCopier();
	virtual ~TextureCopier();

	void setupGL();
	void copy(GLuint sourceTexture, GLuint targetTexture);

protected:
	GLuint framebuffer;

	ShaderProgram*& sp;
};

