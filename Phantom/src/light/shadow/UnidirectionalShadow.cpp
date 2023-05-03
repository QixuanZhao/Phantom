#include "stdafx.h"

void UnidirectionalShadow::setupGL()
{
    Shadow::setupGL();

    /// depth texture
    glGenFramebuffers(1, &depthFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_2D, depthTexture);

    glTextureStorage2D(depthTexture, 1, GL_DEPTH_COMPONENT32F,
        DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT
    );

    static const float borderColourWhite[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    glTextureParameteri(depthTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depthTexture, GL_TEXTURE_MAG_FILTER, linearFilterEnabled ? GL_LINEAR : GL_NEAREST);
    glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTextureParameteri(depthTexture, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTextureParameteri(depthTexture, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);
    glTextureParameterfv(depthTexture, GL_TEXTURE_BORDER_COLOR, borderColourWhite);

    glNamedFramebufferTexture(
        depthFramebuffer,
        GL_DEPTH_ATTACHMENT,
        depthTexture,
        0
    );

    glNamedFramebufferDrawBuffer(depthFramebuffer, GL_NONE);
    glNamedFramebufferReadBuffer(depthFramebuffer, GL_NONE);

    ready = true;
}


void UnidirectionalShadow::updateDepth(ShaderProgram* sp)
{
    if (!isInitialized()) setupGL();

    Shadow::updateShadow();

    sp->set("mixCoefficient", Debug::getInstance().getMix());

    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

    glClear(GL_DEPTH_BUFFER_BIT);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    sp->set("pv", pv);
    sp->set("blending", false);

    glLineWidth(1.0f);
    for (Object3D* object : Data::getInstance().getObjects()) 
        object->draw(*sp);

    sp->set("blending", true);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    sp->set("source", 0);

    glCullFace(GL_BACK);
    for (Object3D* object : Data::getInstance().getObjects())
        object->draw(*sp);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void UnidirectionalShadow::updateShadow(ShaderProgram* sp)
{
    if (!isInitialized()) setupGL();

    sp->set("filterSize", (int)Lighting::lighting.shadowSettings.filterSize);
    sp->set("samplingStratified", Lighting::lighting.shadowSettings.samplingStratified);
    sp->set("samplingEnabled", Lighting::lighting.shadowSettings.samplingEnabled);
    sp->set("samplingSpread", Lighting::lighting.shadowSettings.samplingSpread);
    sp->set("samplingMethod", (int) Lighting::lighting.shadowSettings.samplingMethod);

    glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    sp->set("lightPV", pv);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, depthTexture);
    glTextureParameteri(depthTexture, GL_TEXTURE_MAG_FILTER,
        linearFilterEnabled ? GL_LINEAR : GL_NEAREST);
    sp->set("depthMap", 0);

    glLineWidth(1.0f);

    if (GLConfiguration::configuration.deferredRendering) {
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, GLConfiguration::configuration.gBuffer->positionTexture());
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, GLConfiguration::configuration.gBuffer->depthTexture());
        sp->set("input_position", 1);
        sp->set("input_depth", 2);
        Display::screenQuad->draw(*sp);
    }
    else {
        sp->set("view", Display::display.observerCamera.getView());
        sp->set("projection", Display::display.observerCamera.getProjection());
        glDisable(GL_CULL_FACE);
        for (Object3D* object : Data::getInstance().getObjects())
            object->draw(*sp);

        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        Display::horizon->setColour(glm::mix(Display::display.backgroundColour, vec3(0.5f), 0.1));
        float zoomDistance = Data::getInstance().getZoomDistance();
        if (zoomDistance < 1e-6f) zoomDistance = Display::display.observerCamera.FAR_PLANE_DISTANCE;
        Display::horizon->setScale(vec3(zoomDistance));
        Display::horizon->draw(*sp);
        glDisable(GL_CULL_FACE);
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
