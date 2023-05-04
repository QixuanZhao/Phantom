#include "stdafx.h"

void OmnidirectionalShadow::setupGL()
{
    Shadow::setupGL();

    /// depth texture
    glGenFramebuffers(1, &depthFramebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

    glGenTextures(1, &depthTexture);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);

    glTextureStorage2D(depthTexture, 1, GL_DEPTH_COMPONENT32F,
        DEPTH_MAP_WIDTH, DEPTH_MAP_HEIGHT
    );

    glTextureParameteri(depthTexture, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(depthTexture, GL_TEXTURE_MAG_FILTER, linearFilterEnabled ? GL_LINEAR : GL_NEAREST);

    glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTexture, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glTextureParameteri(depthTexture, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTextureParameteri(depthTexture, GL_TEXTURE_COMPARE_FUNC, GL_GREATER);

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

void OmnidirectionalShadow::updateDepth(ShaderProgram* sp)
{
    if (!isInitialized()) setupGL();

    Shadow::updateShadow();

    sp->set("mixCoefficient", Debug::getInstance().getMix());

    glViewport(0, 0, DEPTH_MAP_WIDTH, DEPTH_MAP_WIDTH);
    glBindFramebuffer(GL_FRAMEBUFFER, depthFramebuffer);

    glClear(GL_DEPTH_BUFFER_BIT);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_FRONT);

    sp->set("blending", false);
    sp->set("far_plane", FAR_PLANE_DISTANCE);
    sp->set("light_position", position);

    for (int i = 0; i < 6; i++) 
        sp->set(QString("pv[%1]").arg(i), pv[i]);
    
    glLineWidth(1.0f);
    for (Object3D* object : Data::getInstance().getObjects()) 
        object->draw(*sp);

    sp->set("blending", true);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);
    sp->set("source", 0);

    glCullFace(GL_BACK);
    for (Object3D* object : Data::getInstance().getObjects())
        object->draw(*sp);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OmnidirectionalShadow::updateShadow(ShaderProgram* sp)
{
    if (!isInitialized()) setupGL();

    sp->set("filterSize", (int) Lighting::lighting.shadowSettings.filterSize);
    sp->set("samplingStratified", Lighting::lighting.shadowSettings.samplingStratified);
    sp->set("samplingEnabled", Lighting::lighting.shadowSettings.samplingEnabled);
    sp->set("samplingSpread", Lighting::lighting.shadowSettings.samplingSpread);
    sp->set("samplingMethod", (int)Lighting::lighting.shadowSettings.samplingMethod);

    glViewport(0, 0, Display::display.TEXTURE_WIDTH, Display::display.TEXTURE_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFramebuffer);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, depthTexture);
    glTextureParameteri(depthTexture,
        GL_TEXTURE_MAG_FILTER,
        linearFilterEnabled ? GL_LINEAR : GL_NEAREST);
    sp->set("depthMap", 0);
    sp->set("lightPosition", position);
    sp->set("far_plane", FAR_PLANE_DISTANCE);

    glLineWidth(1.0f);

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, GLConfiguration::configuration.gBuffer->positionTexture());
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, GLConfiguration::configuration.gBuffer->depthTexture());
    sp->set("input_position", 1);
    sp->set("input_depth", 2);
    Display::screenQuad->draw(*sp);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
