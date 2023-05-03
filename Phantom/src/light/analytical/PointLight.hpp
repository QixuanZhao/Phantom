#pragma once

#ifndef POINT_LIGHT_HPP
#define POINT_LIGHT_HPP

class PointLight : public AttenuateLight {
protected:
    vec3 position; 

public:
    inline PointLight(
        const AttenuateLight& attenuateLight = AttenuateLight(),
        const vec3 & position = vec3(0.0f)
    ) : ReflectiveLight(attenuateLight),
        AttenuateLight(attenuateLight),
        position(position) 
    { }

    inline PointLight(
        const PointLight & pointLight
    ) : ReflectiveLight(pointLight),
        AttenuateLight(pointLight),
        position(pointLight.position)
    { }

    inline virtual ~PointLight() { }

    inline const vec3& getPosition() const { return position; }
    inline virtual void setPosition(const vec3& position) { this->position = position; }

    inline virtual void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
        AttenuateLight::configureShaderProgram(sp, varName);
        sp->set(QStringLiteral("%1.position").arg(varName), position);
    }
};

#endif