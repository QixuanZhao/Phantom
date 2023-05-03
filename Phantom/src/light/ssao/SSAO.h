#pragma once

class SSAO : protected GLF {
public:
	SSAO();
	virtual ~SSAO();

	void render();
	void blur();

	inline GLuint getFramebuffer() const { return framebuffer; }
	inline GLuint getTexture() const { return texture; };
	inline GLint getTextureImageUnit() const { return textureImageUnit; };

protected:
	GLuint framebuffer;
	GLuint texture;
	GLint textureImageUnit;

	ShaderProgram*& sp;
	GBuffer*& gBuffer;
};