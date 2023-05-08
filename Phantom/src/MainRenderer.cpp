#include "stdafx.h"

MainRenderer::MainRenderer():
    orthographicShadowSp(GLConfiguration::configuration.orthographicShadowSp),
    omnidirectionalShadowSp(GLConfiguration::configuration.omnidirectionalShadowSp),
    lightSourceSp(GLConfiguration::configuration.lightSourceSp),
    axisSp(GLConfiguration::configuration.axisSp),
    hdrSp(GLConfiguration::configuration.hdrSp),
    textureSp(GLConfiguration::configuration.textureSp),
    textureArraySp(GLConfiguration::configuration.textureArraySp),
    textureCubeSp(GLConfiguration::configuration.textureCubeSp),
    textureCubeArraySp(GLConfiguration::configuration.textureCubeArraySp),

    orthographicShadowInterpreterDeferredSp(GLConfiguration::configuration.orthographicShadowInterpreterDeferredSp),
    omnidirectionalShadowInterpreterDeferredSp(GLConfiguration::configuration.omnidirectionalShadowInterpreterDeferredSp),
    ssaoSp(GLConfiguration::configuration.ssaoSp),

    pbrSp(GLConfiguration::configuration.pbrSp),

    gBuffer(GLConfiguration::configuration.gBuffer),
    ssao(GLConfiguration::configuration.ssao),

    gaussianBlur(GLConfiguration::configuration.gaussianRGBBlur),
    boxBlur(GLConfiguration::configuration.boxRGBBlur),
    kawaseBlur(GLConfiguration::configuration.kawaseRGBBlur),
    dualBlur(GLConfiguration::configuration.dualRGBBlur),

    bloom(GLConfiguration::configuration.bloom)
{
    //Lighting::lighting.pointLight.setPosition(vec3(0.0f, 0.0f, -5.0f));

	initializeOpenGLFunctions();

    if (!GLConfiguration::configuration.updated) {
        GLConfiguration::configuration.loadShaderPrograms();
        GLConfiguration::configuration.updated = true;
    }

    GLConfiguration::configuration.initializeGBuffer();
    GLConfiguration::configuration.initializeBlur();
    GLConfiguration::configuration.initializeBloom();
    GLConfiguration::configuration.initializeTextureCopier();

    blurTextureImageUnit = GLConfiguration::configuration.textureCount++;
    glGenFramebuffers(1, &blurFramebuffer);
    glGenTextures(1, &blurTexture);

    static const float borderColour[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glBindFramebuffer(GL_FRAMEBUFFER, blurFramebuffer);
    glBindTexture(GL_TEXTURE_2D, blurTexture);
    glTextureStorage2D(blurTexture, 1, GL_RGBA32F,
        Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT
    );

    glTextureParameteri(blurTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(blurTexture, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTextureParameteri(blurTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(blurTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameterfv(blurTexture, GL_TEXTURE_BORDER_COLOR, borderColour);

    glNamedFramebufferTexture(
        blurFramebuffer,
        GL_COLOR_ATTACHMENT0,
        blurTexture, 0
    );

    // HDR
    glGenFramebuffers(1, &hdrFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFramebuffer);

    hdrTextureImageUnit = GLConfiguration::configuration.textureCount++;
    glGenTextures(1, &hdrTexture);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    glTextureStorage2D(hdrTexture, 1, GL_RGBA32F,
        Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
    glTextureParameteri(hdrTexture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(hdrTexture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(hdrTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(hdrTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameterfv(hdrTexture, GL_TEXTURE_BORDER_COLOR, borderColour);

    glGenRenderbuffers(1, &hdrDepthRenderbuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, hdrDepthRenderbuffer);
    glNamedRenderbufferStorage(hdrDepthRenderbuffer, GL_DEPTH_COMPONENT32F,
        Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);

    glNamedFramebufferTexture(hdrFramebuffer, GL_COLOR_ATTACHMENT0,
        hdrTexture, 0);
    glNamedFramebufferRenderbuffer(hdrFramebuffer, GL_DEPTH_ATTACHMENT,
        GL_RENDERBUFFER, hdrDepthRenderbuffer);
    glNamedFramebufferDrawBuffer(hdrFramebuffer, GL_COLOR_ATTACHMENT0);
    // HDR

    //glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, devicePixelRatio() * geometry().width(), devicePixelRatio() * geometry().height());

    //glEnable(GL_BLEND); // I'll be back.
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //Display::display.axes.setVisibility(true);
    Display::display.axes.setLength(Display::display.mouseCamera().FAR_PLANE_DISTANCE);
    Display::display.axes.setupGL();

    Lighting::lightSourceSphere = new Object3D("Light Source Sphere");
    Surface* sphere = new Surface(":/static/res/objects/sphere.obj");
    Lighting::lightSourceSphere->insertSurface(sphere);
    sphere->setupGL();
    sphere->zoom();

    vector<vector<vec3>> quadFaces;
    quadFaces.push_back({
        vec3(-1.0f, 1.0f, 0.0f),
        vec3(-1.0f, -1.0f, 0.0f),
        vec3(1.0f, -1.0f, 0.0f)
    });
    quadFaces.push_back({
        vec3(1.0f, -1.0f, 0.0f),
        vec3(1.0f, 1.0f, 0.0f),
        vec3(-1.0f, 1.0f, 0.0f)
    });

    Display::screenQuad = new Surface(quadFaces);
    Display::screenQuad->setupGL();

    Display::horizon = new Surface(quadFaces);
    Display::horizon->setupGL();
    Display::horizon->setScale(vec3(Display::display.mouseCamera().FAR_PLANE_DISTANCE));

    quadFaces.clear();
    quadFaces.push_back({
        vec3(-1.0f,  1.0f, 1.0f),
        vec3(-1.0f, -1.0f, 1.0f),
        vec3( 1.0f, -1.0f, 1.0f)
    });
    quadFaces.push_back({
        vec3( 1.0f, -1.0f, 1.0f),
        vec3( 1.0f,  1.0f, 1.0f),
        vec3(-1.0f,  1.0f, 1.0f)
    });

    quadFaces.push_back({
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        vec3( 1.0f, -1.0f, -1.0f)
    });
    quadFaces.push_back({
        vec3( 1.0f,  1.0f, -1.0f),
        vec3( 1.0f, -1.0f, -1.0f),
        vec3(-1.0f,  1.0f, -1.0f)
    });

    quadFaces.push_back({
        vec3(-1.0f, 1.0f,  1.0f),
        vec3(-1.0f, 1.0f, -1.0f),
        vec3(1.0f, 1.0f, -1.0f)
        });
    quadFaces.push_back({
        vec3(1.0f, 1.0f, -1.0f),
        vec3(1.0f, 1.0f,  1.0f),
        vec3(-1.0f, 1.0f,  1.0f)
        });

    quadFaces.push_back({
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(1.0f, -1.0f, -1.0f)
        });
    quadFaces.push_back({
        vec3(1.0f, -1.0f,  1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(-1.0f, -1.0f,  1.0f)
        });

    quadFaces.push_back({
        vec3(1.0f, -1.0f,  1.0f),
        vec3(1.0f, -1.0f, -1.0f),
        vec3(1.0f,  1.0f, -1.0f)
        });
    quadFaces.push_back({
        vec3(1.0f,  1.0f, -1.0f),
        vec3(1.0f,  1.0f,  1.0f),
        vec3(1.0f, -1.0f,  1.0f)
        });

    quadFaces.push_back({
        vec3(-1.0f, -1.0f, -1.0f),
        vec3(-1.0f, -1.0f,  1.0f),
        vec3(-1.0f,  1.0f, -1.0f)
        });
    quadFaces.push_back({
        vec3(-1.0f,  1.0f,  1.0f),
        vec3(-1.0f,  1.0f, -1.0f),
        vec3(-1.0f, -1.0f,  1.0f)
        });

    Display::skybox = new Surface(quadFaces);
    Display::skybox->setupGL();
    Display::skybox->setScale(vec3(Display::display.mouseCamera().FAR_PLANE_DISTANCE) / 2.0f);
}

void MainRenderer::render()
{
	qint64 now = QDateTime::currentMSecsSinceEpoch();
    if (!firstFrame) emit fpsCalculated(1000 / float(now - lastFrame));
    deltaTime = now - lastFrame;
	lastFrame = now;

    processKeyboard();
    processMouse();

    if (firstFrame) firstFrame = false;

    if (!GLConfiguration::configuration.updated) {
        GLConfiguration::configuration.loadShaderPrograms();
        GLConfiguration::configuration.updated = true;
    }

    // depth test
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_PROGRAM_POINT_SIZE);

    /// geometry pass
    gBuffer->render();

    if (Lighting::lighting.ssaoEnabled) {
        ssao->render();
        ssao->blur();
    }

    /// shadow mapping
    if (Lighting::lighting.shadowSettings.enabled) {

        for (ParallelLight& pl: Lighting::lighting.parallelLights) {
            Shadow& shadow = pl.getShadow();
            if (!shadow.isUpdated())
                shadow.updateDepth(orthographicShadowSp);
            shadow.updateShadow(orthographicShadowInterpreterDeferredSp);

            shadow.blurShadow(Lighting::lighting.shadowSettings.blurPassCount);
        }

        for (ShadowCastingPointLight& sl : Lighting::lighting.pointLights) {
            Shadow& shadow = sl.getShadow();
            if (!shadow.isUpdated())
                shadow.updateDepth(omnidirectionalShadowSp);
            shadow.updateShadow(omnidirectionalShadowInterpreterDeferredSp);

            shadow.blurShadow(Lighting::lighting.shadowSettings.blurPassCount);
        }

        for (ShadowCastingSpotlight& ss : Lighting::lighting.spotlights) {
            Shadow& shadow = ss.getShadow();
            if (!shadow.isUpdated())
                shadow.updateDepth(orthographicShadowSp);
            shadow.updateShadow(orthographicShadowInterpreterDeferredSp);

            shadow.blurShadow(Lighting::lighting.shadowSettings.blurPassCount);
        }
    }

    QSizeF viewportSize = this->viewportSize * window->effectiveDevicePixelRatio();
    Display::display.mouseCamera().setAspectRatio(1.0f * viewportSize.width() / viewportSize.height());

    // 2. second pass: render to the scene as normal with shadow mapping (using depth map)
    glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, hdrFramebuffer);

    glClearColor(
        Display::display.backgroundColour.r,
        Display::display.backgroundColour.g,
        Display::display.backgroundColour.b,
        1.0f
    );
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //glActiveTexture(GL_TEXTURE1);
    //glBindTexture(GL_TEXTURE_2D, Lighting::lighting.pointLight.getShadow().getShadowTexture());

    pbrSp->set("input_metallic_roughness", 6);
    pbrSp->set("input_distinction_coefficient", 8);

    ShaderProgram* currentSp = pbrSp;

    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, gBuffer->positionTexture());

    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, gBuffer->normalTexture());

    glActiveTexture(GL_TEXTURE4);
    glBindTexture(GL_TEXTURE_2D, gBuffer->albedoTexture());

    glActiveTexture(GL_TEXTURE5);
    glBindTexture(GL_TEXTURE_2D, gBuffer->depthTexture());

    glActiveTexture(GL_TEXTURE6);
    glBindTexture(GL_TEXTURE_2D, gBuffer->metallicRoughnessTexture());

    glActiveTexture(GL_TEXTURE7);
    glBindTexture(GL_TEXTURE_2D, ssao->getTexture());

    glActiveTexture(GL_TEXTURE8);
    glBindTexture(GL_TEXTURE_2D, gBuffer->distinctionCoefficientTexture());

    currentSp->set("parallelLightShadowMap", 0);
    currentSp->set("pointLightShadowMap", 1);

    currentSp->set("input_position", 2);
    currentSp->set("input_normal", 3);
    currentSp->set("input_albedo", 4);
    currentSp->set("input_depth", 5);

    currentSp->set("ambientOcclusionMap", 7);
    currentSp->set("ssaoEnabled", Lighting::lighting.ssaoEnabled);

    Display::display.flashCamera().getFlash().configureShaderProgram(currentSp, "cameraFlash");

    currentSp->set("parallelLightCount", int(Lighting::lighting.parallelLights.size()));

    int index = 0;
    int count = 0;
    for (ParallelLight& pl: Lighting::lighting.parallelLights) {
        pl.configureShaderProgram(currentSp, QStringLiteral("parallelLights[%1]").arg(index));
        glActiveTexture(GL_TEXTURE9 + count);
        glBindTexture(GL_TEXTURE_2D, pl.getShadow().getShadowTexture());
        currentSp->set(QStringLiteral("parallelLightShadowMaps[%1]").arg(index), 9 + count);
        index++, count++;
    }

    currentSp->set("pointLightCount", int(Lighting::lighting.pointLights.size()));

    index = 0;
    for (ShadowCastingPointLight& sl : Lighting::lighting.pointLights) {
        if (index == Lighting::lighting.pickedPointLightIndex) sl.setPosition(Display::display.mouseCamera().getTarget());
        sl.configureShaderProgram(currentSp, QStringLiteral("pointLights[%1]").arg(index));
        glActiveTexture(GL_TEXTURE9 + count);
        glBindTexture(GL_TEXTURE_2D, sl.getShadow().getShadowTexture());
        currentSp->set<int>(QStringLiteral("pointLightShadowMaps[%1]").arg(index), 9 + count);
        index++, count++;
    }

    currentSp->set("spotlightCount", int(Lighting::lighting.spotlights.size()));

    index = 0;
    for (ShadowCastingSpotlight& ss : Lighting::lighting.spotlights) {
        if (index == Lighting::lighting.pickedSpotlightIndex) {
            ss.setPosition(Display::display.mouseCamera().getTarget());
            ss.setDirection(Display::display.mouseCamera().getFront());
        }
        ss.configureShaderProgram(currentSp, QStringLiteral("spotlights[%1]").arg(index));
        glActiveTexture(GL_TEXTURE9 + count);
        glBindTexture(GL_TEXTURE_2D, ss.getShadow().getShadowTexture());
        currentSp->set<int>(QStringLiteral("spotlightShadowMaps[%1]").arg(index), 9 + count);
        index++, count++;
    }

    currentSp->set("ambientLight", Lighting::lighting.ambientLight.getAmbient());

    currentSp->set("cameraPosition", Display::display.mouseCamera().getPosition());
    currentSp->set("cameraFlashOn", Display::display.flashCamera().isFlashOn());

    currentSp->set("reflectingAtBothSide", Lighting::lighting.bilateralReflective);
        
    currentSp->set("shadowEnabled", Lighting::lighting.shadowSettings.enabled);

    Display::screenQuad->draw(*currentSp);

    glBlitNamedFramebuffer(gBuffer->getFramebuffer(), hdrFramebuffer,
        0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT,
        0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT,
        GL_DEPTH_BUFFER_BIT, GL_NEAREST
    );

    /// light sources
    glLineWidth(1.0f);
    Display::display.mouseCamera().configureShaderProgram(lightSourceSp);

    int lightSourceCount = 1;
    lightSourceSp->set("spotlight", false);
    for (ShadowCastingPointLight& sl : Lighting::lighting.pointLights) {
        lightSourceSp->set("lightColour", sl.flux());
        Lighting::lightSourceSphere->setTranslation(sl.getPosition());
        Lighting::lightSourceSphere->draw(*lightSourceSp, lightSourceCount++);
    }

    lightSourceSp->set("spotlight", true);
    for (ShadowCastingSpotlight& ss : Lighting::lighting.spotlights) {
        lightSourceSp->set("lightColour", ss.flux());
        lightSourceSp->set("direction", ss.getDirection());
        lightSourceSp->set("outerCutoff", ss.getOuterCutOff());
        Lighting::lightSourceSphere->setTranslation(ss.getPosition());
        Lighting::lightSourceSphere->draw(*lightSourceSp, lightSourceCount++);
    }

    /// axes
    Display::display.mouseCamera().configureShaderProgram(axisSp);
    Display::display.axes.draw(*axisSp);

    // bloom blur
    if (Lighting::lighting.bloom) bloom->process(hdrTexture);

    // post blur
    switch (Display::display.blurAlgorithm) {
    case GLConfiguration::BlurAlgorithm::Gaussian:
        gaussianBlur->blur(hdrTexture, Display::display.blurPassCount);
        break;
    case GLConfiguration::BlurAlgorithm::Box:
        boxBlur->blur(hdrTexture, Display::display.blurPassCount);
        break;
    case GLConfiguration::BlurAlgorithm::Kawase:
        kawaseBlur->blur(hdrTexture, Display::display.blurPassCount);
        break;
    case GLConfiguration::BlurAlgorithm::Dual:
        dualBlur->blur(hdrTexture, Display::display.blurPassCount);
        break;
    }

    // HDR
    framebufferObject()->bind();
    glViewport(0, 0, viewportSize.width(), viewportSize.height());
    hdrSp->set("exposure", Display::display.exposure);
    hdrSp->set("toneMappingEnabled", Display::display.toneMappingEnabled);
    hdrSp->set("reinhard", Display::display.toneMappingMethod == Display::ToneMappingMethod::Reinhard);
    hdrSp->set("gamma", Display::display.gamma);
    hdrSp->set("hdrBuffer", 0);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, hdrTexture);
    Display::screenQuad->draw(*hdrSp);
    // HDR

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //glViewport(viewportSize.width() - viewportSize.height() / 3, 0, viewportSize.height() / 3, viewportSize.height() / 3);
    //GLint textureImageUnit = Lighting::lighting.parallelLight.getShadow().getDepthTextureImageUnit();
    //GLuint textureName = Lighting::lighting.parallelLight.getShadow().getDepthTextureName();
    //glActiveTexture(GL_TEXTURE0 + textureImageUnit);
    //glBindTexture(GL_TEXTURE_2D, textureName);
    //textureSp->set("alpha", Debug::getInstance().getHubAlpha());
    //textureSp->set("exposure", Display::display.exposure);
    //textureSp->set("textureBuffer", textureImageUnit);
    //Display::screenQuad->draw(*textureSp);

    //glViewport(viewportSize.width() - viewportSize.height() / 3, viewportSize.height() / 3, viewportSize.height() / 3, viewportSize.height() / 3);
    //textureImageUnit = Lighting::lighting.pointLight.getShadow().getDepthTextureImageUnit();
    //textureName = Lighting::lighting.pointLight.getShadow().getDepthTextureName();
    //glActiveTexture(GL_TEXTURE0 + textureImageUnit);
    //glBindTexture(GL_TEXTURE_CUBE_MAP, textureName);
    //textureCubeSp->set("alpha", Debug::getInstance().getHubAlpha());
    ////textureCubeSp->set("mixCoefficient", Debug::getInstance().getMix());
    //textureCubeSp->set("textureBuffer", textureImageUnit);
    //textureCubeSp->set("view", Display::display.camera.getView());
    //textureCubeSp->set("projection", Display::display.camera.getProjection());
    //Display::skybox->draw(*textureCubeSp);

    //glViewport(0, 0, viewportSize.width() / 4, viewportSize.height() / 4);
    //GLint shadowTextureImageUnit = Lighting::lighting.parallelLight.getShadow().getShadowTextureImageUnit();
    //GLuint shadowTextureName = Lighting::lighting.parallelLight.getShadow().getShadowTextureName();
    //glActiveTexture(GL_TEXTURE0 + shadowTextureImageUnit);
    //glBindTexture(GL_TEXTURE_2D, shadowTextureName);
    //textureSp->set("monochrome", true);
    //textureSp->set("normalizing", false);
    //textureSp->set("alpha", Debug::getInstance().getHubAlpha());
    //textureSp->set("exposure", Display::display.exposure);
    //textureSp->set("textureBuffer", shadowTextureImageUnit);
    //Display::screenQuad->draw(*textureSp);

    //glViewport(0, viewportSize.height() / 4, viewportSize.width() / 4, viewportSize.height() / 4);
    //shadowTextureImageUnit = Lighting::lighting.pointLight.getShadow().getShadowTextureImageUnit();
    //shadowTextureName = Lighting::lighting.pointLight.getShadow().getShadowTextureName();
    //glActiveTexture(GL_TEXTURE0 + shadowTextureImageUnit);
    //glBindTexture(GL_TEXTURE_2D, shadowTextureName);
    //textureSp->set("monochrome", true);
    //textureSp->set("normalizing", false);
    //textureSp->set("alpha", Debug::getInstance().getHubAlpha());
    //textureSp->set("exposure", Display::display.exposure);
    //textureSp->set("textureBuffer", shadowTextureImageUnit);
    //Display::screenQuad->draw(*textureSp);

    glActiveTexture(GL_TEXTURE0);
    textureSp->set("textureBuffer", 0);

    //glViewport(3 * viewportSize.width() / 4, 0, viewportSize.width() / 4, viewportSize.height() / 4);
    //glBindTexture(GL_TEXTURE_2D, gBuffer->positionTexture());
    //textureSp->set("monochrome", false);
    //textureSp->set("normalizing", false);
    //textureSp->set("alpha", Debug::getInstance().getHubAlpha());
    //textureSp->set("exposure", Display::display.exposure / 100.0f);
    //Display::screenQuad->draw(*textureSp);

    //glViewport(3 * viewportSize.width() / 4, viewportSize.height() / 4, viewportSize.width() / 4, viewportSize.height() / 4);
    //glBindTexture(GL_TEXTURE_2D, gBuffer->normalTexture());
    //textureSp->set("monochrome", false);
    //textureSp->set("normalizing", true);
    //textureSp->set("alpha", Debug::getInstance().getHubAlpha());
    //textureSp->set("exposure", Display::display.exposure);
    //Display::screenQuad->draw(*textureSp);

    //glViewport(viewportSize.width() / 2, 0, viewportSize.width() / 4, viewportSize.height() / 4);
    //glBindTexture(GL_TEXTURE_2D, gBuffer->distinctionCoefficientTexture());
    //textureSp->set("monochrome", false);
    //textureSp->set("normalizing", false);
    //textureSp->set("alpha", Debug::getInstance().getHubAlpha());
    //textureSp->set("exposure", Display::display.exposure);
    //Display::screenQuad->draw(*textureSp);

    //glViewport(viewportSize.width() / 2, viewportSize.height() / 4, viewportSize.width() / 4, viewportSize.height() / 4);
    //glBindTexture(GL_TEXTURE_2D, gBuffer->metallicRoughnessTexture());
    //textureSp->set("monochrome", false);
    //textureSp->set("normalizing", false);
    //textureSp->set("alpha", Debug::getInstance().getHubAlpha());
    //textureSp->set("exposure", Display::display.exposure);
    //Display::screenQuad->draw(*textureSp);

    if (Lighting::lighting.spotlights.size() > 0) {
        glViewport(0, 0, viewportSize.width() / 2, viewportSize.height() / 2);
        glBindTexture(GL_TEXTURE_2D, Lighting::lighting.spotlights.front().getShadow().getDepthTexture());
        textureSp->set("monochrome", true);
        textureSp->set("normalizing", false);
        textureSp->set("alpha", Debug::getInstance().getHubAlpha());
        textureSp->set("exposure", Display::display.exposure);
        Display::screenQuad->draw(*textureSp);

        glViewport(viewportSize.width() / 2, 0, viewportSize.width() / 2, viewportSize.height() / 2);
        glBindTexture(GL_TEXTURE_2D, Lighting::lighting.spotlights.front().getShadow().getShadowTexture());
        textureSp->set("monochrome", true);
        textureSp->set("normalizing", false);
        textureSp->set("alpha", Debug::getInstance().getHubAlpha());
        textureSp->set("exposure", Display::display.exposure);
        Display::screenQuad->draw(*textureSp);
    }

    glDisable(GL_BLEND);

	update();
}

void MainRenderer::processKeyboard()
{
    if (!Display::display.observerMode && Control::instance().catchMouse) {
        int horizontalMove = 0;
        int verticalMove = 0;

        float dtime = deltaTime / 100.0f;

        if (Control::instance().pressedKeys.contains(Qt::Key::Key_Shift)) dtime *= 10.0f;

        if (Control::instance().pressedKeys.contains(Qt::Key::Key_W) ||
            Control::instance().pressedKeys.contains(Qt::Key::Key_Up)) {
            verticalMove++;
        }

        if (Control::instance().pressedKeys.contains(Qt::Key::Key_S) ||
            Control::instance().pressedKeys.contains(Qt::Key::Key_Down)) {
            verticalMove--;
        }

        if (Control::instance().pressedKeys.contains(Qt::Key::Key_D) ||
            Control::instance().pressedKeys.contains(Qt::Key::Key_Right)) {
            horizontalMove++;
        }

        if (Control::instance().pressedKeys.contains(Qt::Key::Key_A) ||
            Control::instance().pressedKeys.contains(Qt::Key::Key_Left)) {
            horizontalMove--;
        }

        if (horizontalMove > 0) 
            if (verticalMove > 0)
                Display::display.fpsCamera.move(FPSCamera::Direction::FORWARD_RIGHT, dtime);
            else if (verticalMove < 0)
                Display::display.fpsCamera.move(FPSCamera::Direction::BACKWARD_RIGHT, dtime);
            else
                Display::display.fpsCamera.move(FPSCamera::Direction::RIGHT, dtime);
        else if (horizontalMove < 0)
            if (verticalMove > 0)
                Display::display.fpsCamera.move(FPSCamera::Direction::FORWARD_LEFT, dtime);
            else if (verticalMove < 0)
                Display::display.fpsCamera.move(FPSCamera::Direction::BACKWARD_LEFT, dtime);
            else
                Display::display.fpsCamera.move(FPSCamera::Direction::LEFT, dtime);
        else
            if (verticalMove > 0)
                Display::display.fpsCamera.move(FPSCamera::Direction::FORWARD, dtime);
            else if (verticalMove < 0)
                Display::display.fpsCamera.move(FPSCamera::Direction::BACKWARD, dtime);

    }
}

void MainRenderer::processMouse()
{
    QPoint pos = QCursor::pos();
    if (!Display::display.observerMode && Control::instance().catchMouse) {
        Display::display.fpsCamera.mouse(pos.x(), pos.y());

        QPoint center = Control::instance().center;
        Display::display.fpsCamera.setMouse(center.x(), center.y());
        QCursor::setPos(center);
    }
    else if (Display::display.observerMode) {
        if (Control::instance().pressedButtons.contains(Qt::MouseButton::LeftButton))
            Display::display.observerCamera.unlockView();
        else Display::display.observerCamera.lockView();

        if (Control::instance().pressedButtons.contains(Qt::MouseButton::RightButton))
            Display::display.observerCamera.unlockTranslation();
        else Display::display.observerCamera.lockTranslation();

        Display::display.observerCamera.mouse(pos.x(), pos.y());
    }
}
