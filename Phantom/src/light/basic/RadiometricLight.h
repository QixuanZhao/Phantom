#pragma once

#ifndef RADIOMETRIC_LIGHT_H
#define RADIOMETRIC_LIGHT_H

class RadiometricLight {
protected:
    vec3 radiantFlux;

    vec3 colour;
    float power;

public:
    inline RadiometricLight (
        const vec3& flux = vec3(1.0f)
    ) : radiantFlux(flux), colour(flux), power(1.0f) { }

    inline RadiometricLight (
        const RadiometricLight& light
    ) : radiantFlux(light.radiantFlux),
        colour(light.colour),
        power(light.power)
    { }

    inline virtual ~RadiometricLight() { }

    inline virtual vec3 getRadiantFlux() const { return radiantFlux; }

    inline vec3 getColour() const { return colour; }
    inline float getPower() const { return power; }
    
    inline void setColour(const vec3& colour) {
        this->colour = colour;
        radiantFlux = colour * power;
    }

    inline void setPower(const float& power) {
        this->power = power;
        radiantFlux = colour * power;
    }

    inline virtual void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
        sp->set(QStringLiteral("%1.flux").arg(varName), radiantFlux);
    }
};

#endif