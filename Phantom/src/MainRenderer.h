#pragma once

class RenderingFramebufferObject;

class MainRenderer : public QObject,
	public QQuickFramebufferObject::Renderer,
	protected GLF
{
	Q_OBJECT
public:
	MainRenderer();

public slots:
    inline void setWindow(QQuickWindow* window) { this->window = window; }
    inline void setViewportSize(const QSizeF& size) { viewportSize = size; }

signals:
	void fpsCalculated(qint32 fps);

protected:
	virtual void render() override;

    void processKeyboard();

    GLuint hdrFramebuffer;
    GLuint hdrDepthRenderbuffer;
    GLuint hdrTexture;
    GLint hdrTextureImageUnit;
    GLuint blurFramebuffer;
    GLuint blurTexture;
    GLint blurTextureImageUnit;

    ShaderProgram*& sp;
    ShaderProgram*& orthographicShadowSp;
    ShaderProgram*& orthographicShadowInterpreterSp;
    ShaderProgram*& omnidirectionalShadowSp;
    ShaderProgram*& omnidirectionalShadowInterpreterSp;
    ShaderProgram*& lightSourceSp;
    ShaderProgram*& axisSp;
    ShaderProgram*& hdrSp;
    ShaderProgram*& textureSp;
    ShaderProgram*& textureArraySp;
    ShaderProgram*& textureCubeSp;
    ShaderProgram*& textureCubeArraySp;

    ShaderProgram*& lightingDeferredSp;
    ShaderProgram*& orthographicShadowInterpreterDeferredSp;
    ShaderProgram*& omnidirectionalShadowInterpreterDeferredSp;
    ShaderProgram*& ssaoSp;

    ShaderProgram*& pbrSp;

    GBuffer*& gBuffer;
    SSAO*& ssao;

    GaussianBlur<GL_RGB32F>*& gaussianBlur;
    BoxBlur<GL_RGB32F>*& boxBlur;
    KawaseBlur<GL_RGB32F>*& kawaseBlur;
    DualBlur<GL_RGB32F>*& dualBlur;

    Bloom*& bloom;

private:
    bool firstFrame = true;
	qint64 lastFrame = 0;
    qint64 deltaTime = 1;

    QQuickWindow* window = nullptr;
    QSizeF viewportSize;
};