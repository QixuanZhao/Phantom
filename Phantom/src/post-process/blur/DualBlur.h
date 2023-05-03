#pragma once

class ShaderProgram;

template<GLenum internalFormat>
class DualBlur : public Blur<internalFormat> {
public:
	DualBlur();
	virtual ~DualBlur();

	virtual void blur(GLuint imageTexture, GLuint rounds) override;
protected:
	void setupGL();

	ShaderProgram*& upSp;
	ShaderProgram*& downSp = Blur<internalFormat>::sp;

	GLuint textures[10];

	TextureCopier<internalFormat>*& copier;
};
