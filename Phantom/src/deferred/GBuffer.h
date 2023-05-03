#pragma once

class GBuffer : protected GLF {
public:
    GBuffer();
    virtual ~GBuffer();
    void render();

    inline GLuint positionTexture() const { return textures[0]; }
    inline GLuint normalTexture() const { return textures[1]; }
    inline GLuint albedoTexture() const { return textures[2]; }
    inline GLuint viewSpacePositionTexture() const { return textures[3]; }
    inline GLuint viewSpaceNormalTexture() const { return textures[4]; }
    inline GLuint metallicRoughnessTexture() const { return textures[5]; }
    inline GLuint distinctionCoefficientTexture() const { return textures[6]; }
    inline GLuint depthTexture() const { return textures[count - 1]; }

    inline GLint positionTextureImageUnit() const { return textureImageUnits[0]; }
    inline GLint normalTextureImageUnit() const { return textureImageUnits[1]; }
    inline GLint albedoTextureImageUnit() const { return textureImageUnits[2]; }
    inline GLint viewSpacePositionTextureImageUnit() const { return textureImageUnits[3]; }
    inline GLint viewSpaceNormalTextureImageUnit() const { return textureImageUnits[4]; }
    inline GLint metallicRoughnessTextureImageUnit() const { return textureImageUnits[5]; }
    inline GLint depthTextureImageUnit() const { return textureImageUnits[count - 1]; }

    inline GLuint getFramebuffer() const { return framebuffer; }

protected:
    GLuint framebuffer;
    GLuint textures[8];
    GLint textureImageUnits[8];
    const int count = sizeof(textures) / sizeof(GLuint);
    
    ShaderProgram*& sp;
};