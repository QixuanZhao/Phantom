#pragma once

class Bloom : protected GLF {
public:
	Bloom();
	virtual ~Bloom();

	void process(GLuint sourceTexture);

protected:
	GLuint framebuffer;
	GLuint textures[10];

	ShaderProgram*& downSp;
	ShaderProgram*& upSp;
};