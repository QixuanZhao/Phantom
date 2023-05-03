#pragma once

/**
 * @file    CoordinateSystem.h
 * @brief   This file declares the CoordinateSystem class. <br/>
 *          此文件声明 CoordinateSystem 类。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef COORDINATE_SYSTEM_H
#define COORDINATE_SYSTEM_H

/**
 * @class CoordinateSystem
 * @brief Coordinate system class. <br/> 坐标系类
 * @details This class represents the coordinate system to be rendered. <br/>
 *			这个类表示要被渲染的坐标系
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
class CoordinateSystem : protected GLF
{
protected:
    vec3 origin;

    vec3 xAxis;
    vec3 yAxis;
    vec3 zAxis;

    float length;

    bool visible = false;
    GLuint vao;
    GLuint vbo;

public:
    CoordinateSystem();
    CoordinateSystem(const vec3& origin, const vec3& xAxis, const vec3& yAxis, const vec3& zAxis);
    
    inline void setOrigin(const vec3& origin) { this->origin = origin; }
    inline void setXAxis(const vec3& xAxis) { this->xAxis = xAxis; }
    inline void setYAxis(const vec3& yAxis) { this->yAxis = yAxis; }
    inline void setZAxis(const vec3& zAxis) { this->zAxis = zAxis; }

    inline void setLength(const float& length) { this->length = length; }

    inline void setVisibility(const bool& visible) { this->visible = visible; }

    inline const vec3& getOrigin() const { return origin; }
    inline const vec3& getXAxis() const { return xAxis; }
    inline const vec3& getYAxis() const { return yAxis; }
    inline const vec3& getZAxis() const { return zAxis; }
    inline float getLength() const { return length; }

    inline bool isVisible() { return visible; }

    inline void draw(ShaderProgram& sp) {
        if (!visible) return;

        sp.use();
        mat4 model = glm::scale(mat4(1.0f), vec3(length));
        model = glm::translate(model, origin);
        sp.set("model", model);
        glDepthFunc(GL_ALWAYS);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glLineWidth(5.0f);
        glBindVertexArray(vao);
        glDrawArrays(GL_LINES, 0, 6);
        glBindVertexArray(0);
        glDisable(GL_BLEND);
        glDepthFunc(GL_LEQUAL);
    }
    /**
     * @brief sets up OpenGL <br/> 设置 OpenGL
     * @details set axes data, vao, and vbo <br/> 设置坐标轴数据、vao和vbo
     * @param glf openGl functions class pointer <br/> openGL 函数类指针
     */
    void setupGL();
};

#endif