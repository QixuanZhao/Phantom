#pragma once

#ifndef ATTENUATE_LIGHT_H
#define ATTENUATE_LIGHT_H

class AttenuateLight : virtual public ReflectiveLight {
protected:
    float constant;
    float linear;
    float quadratic;
public:
    inline AttenuateLight(
        const ReflectiveLight& l = ReflectiveLight(),
        const float& constant = 1.0f,
        const float& linear = 0.0f,
        const float& quadratic = 0.0f
    ) : ReflectiveLight(l),
        constant(constant),
        linear(linear),
        quadratic(quadratic)
    { }

    inline AttenuateLight(
        const AttenuateLight& attenuateLight
    ) : ReflectiveLight(attenuateLight), 
        constant(attenuateLight.constant),
        linear(attenuateLight.linear),
        quadratic(attenuateLight.quadratic)
    { }

    inline virtual ~AttenuateLight() { }

    inline const float& getConstant() const { return constant; }
    inline const float& getLinear() const { return linear; }
    inline const float& getQuadratic() const { return quadratic; }

    inline void setConstant(const float& constant) { this->constant = constant; }
    inline void setLinear(const float& linear) { this->linear = linear; }
    inline void setQuadratic(const float& quadratic) { this->quadratic = quadratic; }

    inline virtual void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
        ReflectiveLight::configureShaderProgram(sp, varName);
        sp->set(QStringLiteral("%1.constant").arg(varName), constant);
        sp->set(QStringLiteral("%1.linear").arg(varName), linear);
        sp->set(QStringLiteral("%1.quadratic").arg(varName), quadratic);
    }
};

#endif
