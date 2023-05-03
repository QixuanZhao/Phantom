#include "stdafx.h"
#pragma once

/**
 * @file    AmbientLight.hpp
 * @brief   This file defines the AmbientLight class. <br/>
 *          此文件定义 AmbientLight 类。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef AMBIENT_LIGHT_HPP
#define AMBIENT_LIGHT_HPP

/**
 * @class   AmbientLight
 * @brief   Ambient light class. <br/> 环境光类
 * @details Ambient light indiscriminately lights all fragments. <br/>
 *          环境光无差别地照亮所有片段。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
class AmbientLight {
protected:
    vec3 ambient; 
                  
public:
    inline AmbientLight(const vec3& ambient = vec3(0.3f)) : ambient(ambient) { }
    inline virtual ~AmbientLight() { }

    inline void setAmbient(const vec3& ambient) { this->ambient = ambient; }
    inline const vec3& getAmbient() const { return ambient; }

    inline virtual void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
        sp->set(QStringLiteral("%1.ambient").arg(varName), ambient);
    }
};

#endif