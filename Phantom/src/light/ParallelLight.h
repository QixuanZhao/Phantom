#pragma once

#ifndef PARALLEL_LIGHT_H
#define PARALLEL_LIGHT_H

class ParallelLight : public DirectionalLight {
protected:
    OrthographicShadow shadow;

public:
    inline ParallelLight(
        const DirectionalLight& directionalLight = DirectionalLight()
    ) : ReflectiveLight(directionalLight),
        DirectionalLight(directionalLight)
    { }

    inline ParallelLight(
        const ParallelLight& parallelLight
    ) : ReflectiveLight(parallelLight),
        DirectionalLight(parallelLight),
        shadow(parallelLight.shadow)
    { }

    inline OrthographicShadow& getShadow() { return shadow; }

    void setDirection(const vec3& direction) override;
};

#endif
