#include "stdafx.h"

GLConfiguration GLConfiguration::configuration;

void GLConfiguration::loadOrthographicalShadowShaderProgram()
{
    if (orthographicShadowSp) delete orthographicShadowSp;

    orthographicShadowSp = new ShaderProgram(
        TextFile("res/shaders/orthographic-shadow.vert").getContent(),
        TextFile("res/shaders/orthographic-shadow.frag").getContent()
    );
}

void GLConfiguration::loadOmnidirectionalShadowShaderProgram()
{
    if (omnidirectionalShadowSp) delete omnidirectionalShadowSp;

    omnidirectionalShadowSp = new ShaderProgram(
        TextFile("res/shaders/omnidirectional-shadow.vert").getContent(),
        TextFile("res/shaders/omnidirectional-shadow.geom").getContent(),
        TextFile("res/shaders/omnidirectional-shadow.frag").getContent()
    );
}

void GLConfiguration::loadLightSourceShaderProgram()
{
    if (lightSourceSp) delete lightSourceSp;

    lightSourceSp = new ShaderProgram(
        TextFile("res/shaders/light-source.vert").getContent(),
        TextFile("res/shaders/light-source.frag").getContent()
    );
}

void GLConfiguration::loadAxisShaderProgram()
{
    if (axisSp) delete axisSp;

    axisSp = new ShaderProgram(
        TextFile("res/shaders/axis.vert").getContent(),
        TextFile("res/shaders/axis.frag").getContent()
    );
}

void GLConfiguration::loadBlurShaderProgram()
{
    if (gaussianBlurRGBSp) delete gaussianBlurRGBSp;
    if (gaussianBlurRGSp) delete gaussianBlurRGSp;
    if (gaussianBlurRSp) delete gaussianBlurRSp;

    if (boxBlurRGBSp) delete boxBlurRGBSp;
    if (boxBlurRGSp) delete boxBlurRGSp;
    if (boxBlurRSp) delete boxBlurRSp;

    if (kawaseBlurRGBSp) delete kawaseBlurRGBSp;
    if (kawaseBlurRGSp) delete kawaseBlurRGSp;
    if (kawaseBlurRSp) delete kawaseBlurRSp;

    if (dualDownBlurRGBSp) delete dualDownBlurRGBSp;
    if (dualDownBlurRGSp) delete dualDownBlurRGSp;
    if (dualDownBlurRSp) delete dualDownBlurRSp;

    if (dualUpBlurRGBSp) delete dualUpBlurRGBSp;
    if (dualUpBlurRGSp) delete dualUpBlurRGSp;
    if (dualUpBlurRSp) delete dualUpBlurRSp;

    const QString vertexShader = TextFile("res/shaders/blur.vert").getContent();
    const QString dualBlurVertexShader = TextFile("res/shaders/bloom.vert").getContent();

    gaussianBlurRGBSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-gaussian-rgb.frag").getContent()
    );

    gaussianBlurRGSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-gaussian-rg.frag").getContent()
    );

    gaussianBlurRSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-gaussian-r.frag").getContent()
    );

    boxBlurRGBSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-box-rgb.frag").getContent()
    );

    boxBlurRGSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-box-rg.frag").getContent()
    );

    boxBlurRSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-box-r.frag").getContent()
    );

    kawaseBlurRGBSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-kawase-rgb.frag").getContent()
    );

    kawaseBlurRGSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-kawase-rg.frag").getContent()
    );

    kawaseBlurRSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/blur-kawase-r.frag").getContent()
    );

    dualDownBlurRGBSp = new ShaderProgram(
        dualBlurVertexShader,
        TextFile("res/shaders/blur-dual-down-rgb.frag").getContent()
    );

    dualDownBlurRGSp = new ShaderProgram(
        dualBlurVertexShader,
        TextFile("res/shaders/blur-dual-down-rg.frag").getContent()
    );

    dualDownBlurRSp = new ShaderProgram(
        dualBlurVertexShader,
        TextFile("res/shaders/blur-dual-down-r.frag").getContent()
    );

    dualUpBlurRGBSp = new ShaderProgram(
        dualBlurVertexShader,
        TextFile("res/shaders/blur-dual-up-rgb.frag").getContent()
    );

    dualUpBlurRGSp = new ShaderProgram(
        dualBlurVertexShader,
        TextFile("res/shaders/blur-dual-up-rg.frag").getContent()
    );

    dualUpBlurRSp = new ShaderProgram(
        dualBlurVertexShader,
        TextFile("res/shaders/blur-dual-up-r.frag").getContent()
    );
}

void GLConfiguration::loadCopyShaderProgram()
{
    if (copyRGBSp) delete copyRGBSp;
    if (copyRGSp) delete copyRGSp;
    if (copyRSp) delete copyRSp;

    const QString vertexShader = TextFile("res/shaders/blur.vert").getContent();

    copyRGBSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/copy-rgb.frag").getContent()
    );

    copyRGSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/copy-rg.frag").getContent()
    );

    copyRSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/copy-r.frag").getContent()
    );
}

void GLConfiguration::loadHDRShaderProgram()
{
    if (hdrSp) delete hdrSp;

    hdrSp = new ShaderProgram(
        TextFile("res/shaders/hdr.vert").getContent(),
        TextFile("res/shaders/hdr.frag").getContent()
    );
}

void GLConfiguration::loadBloomShaderProgram()
{
    if (bloomDownSp) delete bloomDownSp;
    if (bloomUpSp) delete bloomUpSp;

    const QString vertexShader = TextFile("res/shaders/bloom.vert").getContent();

    bloomDownSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/bloom-down.frag").getContent()
    );

    bloomUpSp = new ShaderProgram(
        vertexShader,
        TextFile("res/shaders/bloom-up.frag").getContent()
    );
}

void GLConfiguration::loadTextureShaderProgram()
{
    if (textureSp) delete textureSp;

    textureSp = new ShaderProgram(
        TextFile("res/shaders/texture.vert").getContent(),
        TextFile("res/shaders/texture.frag").getContent()
    );
}

void GLConfiguration::loadTextureArrayShaderProgram()
{
    if (textureArraySp) delete textureArraySp;

    textureArraySp = new ShaderProgram(
        TextFile("res/shaders/texture-array.vert").getContent(),
        TextFile("res/shaders/texture-array.frag").getContent()
    );
}

void GLConfiguration::loadTextureCubeShaderProgram()
{
    if (textureCubeSp) delete textureCubeSp;

    textureCubeSp = new ShaderProgram(
        TextFile("res/shaders/texture-cube.vert").getContent(),
        TextFile("res/shaders/texture-cube.frag").getContent()
    );
}

void GLConfiguration::loadTextureCubeArrayShaderProgram()
{
    if (textureCubeArraySp) delete textureCubeArraySp;

    textureCubeArraySp = new ShaderProgram(
        TextFile("res/shaders/texture-cube-array.vert").getContent(),
        TextFile("res/shaders/texture-cube-array.frag").getContent()
    );
}

void GLConfiguration::loadGeometryPassShaderProgram()
{
    if (gpSp) delete gpSp;

    gpSp = new ShaderProgram(
        TextFile("res/shaders/geometry-pass.vert").getContent(),
        TextFile("res/shaders/geometry-pass.frag").getContent()
    );
}

void GLConfiguration::loadOrthographicShadowInterpreterDeferredShaderProgram()
{
    if (orthographicShadowInterpreterDeferredSp) delete orthographicShadowInterpreterDeferredSp;

    orthographicShadowInterpreterDeferredSp = new ShaderProgram(
        TextFile("res/shaders/orthographic-shadow-interpreter-deferred.vert").getContent(),
        TextFile("res/shaders/orthographic-shadow-interpreter-deferred.frag").getContent()
    );
}

void GLConfiguration::loadOmnidirectionalShadowInterpreterDeferredShaderProgram()
{
    if (omnidirectionalShadowInterpreterDeferredSp) delete omnidirectionalShadowInterpreterDeferredSp;

    omnidirectionalShadowInterpreterDeferredSp = new ShaderProgram(
        TextFile("res/shaders/omnidirectional-shadow-interpreter-deferred.vert").getContent(),
        TextFile("res/shaders/omnidirectional-shadow-interpreter-deferred.frag").getContent()
    );
}

void GLConfiguration::loadSSAOShaderProgram()
{
    if (ssaoSp) delete ssaoSp;

    ssaoSp = new ShaderProgram(
        TextFile("res/shaders/ssao.vert").getContent(),
        TextFile("res/shaders/ssao.frag").getContent()
    );
}

void GLConfiguration::loadPBRShaderProgram()
{
    if (pbrSp) delete pbrSp;

    pbrSp = new ShaderProgram(
        TextFile("res/shaders/lighting-deferred.vert").getContent(),
        TextFile("res/shaders/lighting-pbr-deferred.frag").getContent()
    );
}

void GLConfiguration::initializeGBuffer()
{
    gBuffer = new GBuffer();
    ssao = new SSAO();
}

void GLConfiguration::initializeTextureCopier()
{
    rgbCopier = new TextureCopier<GL_RGB32F>();
    rgCopier = new TextureCopier<GL_RG32F>();
    rCopier = new TextureCopier<GL_R32F>();
}

void GLConfiguration::initializeBlur()
{
    gaussianRGBBlur = new GaussianBlur<GL_RGB32F>();
    gaussianRBlur = new GaussianBlur<GL_R32F>();
    boxRGBBlur = new BoxBlur<GL_RGB32F>();
    boxRBlur = new BoxBlur<GL_R32F>();
    kawaseRGBBlur = new KawaseBlur<GL_RGB32F>();
    kawaseRBlur = new KawaseBlur<GL_R32F>();
    dualRGBBlur = new DualBlur<GL_RGB32F>();
    dualRBlur = new DualBlur<GL_R32F>();
}

void GLConfiguration::initializeBloom()
{
    bloom = new Bloom();
}
