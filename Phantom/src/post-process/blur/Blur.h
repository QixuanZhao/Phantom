#pragma once

class ShaderProgram;

template<GLenum internalFormat>
class Blur : protected GLF {
public:
	Blur(ShaderProgram*& sp);
	virtual ~Blur();

	virtual void blur(GLuint imageTexture, GLuint rounds = 1);

protected:
	GLuint texture;
	GLuint framebuffer;

	ShaderProgram*& sp;
};
