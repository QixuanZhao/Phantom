#include "stdafx.h"
#pragma once

/**
 * @file    MouseCamera.hpp
 * @brief   This file defines the MouseCamera class. <br/>
 *          此文件定义 MouseCamera 类。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef MOUSE_CAMERA_HPP
#define MOUSE_CAMERA_HPP

/**
 * @class MouseCamera
 * @brief Abstract class representing a camera reacting to mouse events. <br/>
 *        表示一个对鼠标做出反应的相机。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
class MouseCamera : virtual public Camera {
protected:
    double lastX = 0.0;
    double lastY = 0.0;

    float sensitivity;
public:
    inline MouseCamera(
        const Camera& camera,
        const float& sensitivity = 1.13f
    ) : Camera(camera),
        sensitivity(sensitivity)
    { }

    inline MouseCamera(
        const float& pitch = -35.0f,
        const float& yaw = -135.0f,
        const float& roll = 0.0f,
        const vec3& position = vec3(0.0f),
        const float& fov = 45.0f,
        const float& aspectRatio = 16.0f / 9.0f,
        const float& sensitivity = 0.13f
    ) : Camera(
            pitch,
            yaw,
            roll,
            position,
            fov,
            aspectRatio
        ), 
        sensitivity(sensitivity)
    { }

    inline void setSensitivity(const float& sensitivity) { this->sensitivity = sensitivity; }
    inline const float& getSensitivity() { return sensitivity; }
    
    inline void setMouse(double x, double y) { lastX = x; lastY = y; }
    inline double getLastX() const { return lastX; }
    inline double getLastY() const { return lastY; }

    virtual void mouse(double x, double y) = 0;
    virtual void scroll(double dy) = 0;
};
#endif