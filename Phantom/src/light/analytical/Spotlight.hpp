#pragma once

#ifndef SPOTLIGHT_HPP
#define SPOTLIGHT_HPP

class Spotlight : public PointLight, public DirectionalLight {
protected:
    float outerCutOff;
    float innerCutOff;

public:
    inline Spotlight(
        const PointLight& pointLight = PointLight(),
        const DirectionalLight& directionalLight = DirectionalLight(),
        const float& outerCutOff = glm::radians(15.0f),
        const float& innerCutOff = glm::radians(10.0f)
    ) : ReflectiveLight(pointLight),
        PointLight(pointLight),
        DirectionalLight(directionalLight),
        outerCutOff(outerCutOff),
        innerCutOff(innerCutOff)
    { }

    /// @brief copy constructor <br/> 拷贝构造函数
    inline Spotlight(
        const Spotlight& spotlight
    ) : ReflectiveLight(spotlight),
        PointLight(spotlight),
        DirectionalLight(spotlight),
        outerCutOff(spotlight.outerCutOff),
        innerCutOff(spotlight.innerCutOff)
    { }

    inline const float& getOuterCutOff() const { return outerCutOff; }
    inline const float& getInnerCutOff() const { return innerCutOff; }
    inline virtual void setOuterCutOff(const float& outerCutOff) { this->outerCutOff = outerCutOff; }
    inline void setInnerCutOff(const float& innerCutOff) { this->innerCutOff = innerCutOff; }

    inline virtual void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
        PointLight::configureShaderProgram(sp, varName);
        DirectionalLight::configureShaderProgram(sp, varName);
        sp->set(QStringLiteral("%1.outerCutOff").arg(varName), outerCutOff);
        sp->set(QStringLiteral("%1.innerCutOff").arg(varName), innerCutOff);
    }
};

#endif