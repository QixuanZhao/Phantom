#include "stdafx.h"
#pragma once

#ifndef COMPOSITE_LIGHT_HPP
#define COMPOSITE_LIGHT_HPP

class CompositeLight {
protected:
    vec3 diffuse;
    vec3 specular;

public:
    inline CompositeLight(
        const vec3 & diffuse = vec3(1.0f), 
        const vec3 & specular = vec3(1.0f)
    ) : diffuse(diffuse), specular(specular) { }

    inline CompositeLight(
        const CompositeLight & light
    ) : diffuse(light.diffuse),
        specular(light.specular)
    { }

    inline virtual ~CompositeLight() { }

    inline const vec3& getDiffuse() const { return diffuse; }
    inline const vec3& getSpecular() const { return specular; }

    inline void setDiffuse(const vec3& diffuse) { this->diffuse = diffuse; }
    inline void setSpecular(const vec3& specular) { this->specular = specular; }

    inline virtual void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
        sp->set(QStringLiteral("%1.diffuse").arg(varName), diffuse);
        sp->set(QStringLiteral("%1.specular").arg(varName), specular);
    }
};

#endif