#include "stdafx.h"
#pragma once

/**
 * @file    FPSCamera.hpp
 * @brief   This file defines the FPSCamera class. <br/>
 *          此文件定义 FPSCamera 类。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef FPS_CAMERA_HPP
#define FPS_CAMERA_HPP

using glm::vec3;
using glm::normalize;
using glm::cross;

/**
 * @class FPSCamera
 * @brief First Person Shooting (FPS) camera <br/> FPS 相机
 * @details This class defines a camera accepting mouse coordinates and moving by a specified speed and direction. <br/>
 *          这个类定义了一个相机，可接受光标坐标、按照特定速度和方向移动。
 * 
 *          This is exactly the FPS game camera. <br/>
 *          就是 FPS 游戏中的相机。
 * @author 赵启轩 <QixuanZhao@outlook.com>
 */
class FPSCamera : public MouseCamera {
private:
    bool verticalMoveLock = true;

protected:
    float speed;
    bool firstMouse = true;

public: 
    enum class Direction {
        FORWARD, 
        BACKWARD,
        LEFT,
        RIGHT,
        FORWARD_LEFT,
        FORWARD_RIGHT,
        BACKWARD_LEFT,
        BACKWARD_RIGHT
    };

    inline FPSCamera(
        const float& pitch = -35.0f,
        const float& yaw = -135.0f,
        const float& roll = 0.0f, 
        const vec3 & position = vec3(0.0f),
        const float& fov = 45.0f, 
        const float& aspectRatio = 16.0f / 9.0f,
        const float& sensitivity = 0.03f, 
        const float& speed = 2.5f
    ) : FPSCamera(
            Camera(
                pitch,
                yaw,
                roll,
                position,
                fov,
                aspectRatio
            ),
            sensitivity,
            speed
        )
    { }

    inline FPSCamera(
        const Camera& camera,
        const float& sensitivity = 0.03f,
        const float& speed = 2.5f
    ) : Camera(camera),
        MouseCamera(camera, sensitivity),
        speed(speed)
    { }
    
    inline FPSCamera(const FPSCamera & fpsCamera) :
        Camera(fpsCamera),
        MouseCamera(fpsCamera),
        speed(fpsCamera.speed)
    { }

    virtual void mouse (double x, double y) override;
    virtual void scroll (double dy) override;

    virtual void move (Direction direction);
    virtual void move (Direction direction, float dtime);
};  


inline void FPSCamera::mouse (double x, double y) {
    if (firstMouse) {
        lastX = x;
        lastY = y;
        firstMouse = false;
    }

    float dx = float(x - lastX);
    float dy = float(lastY - y);
    lastX = x;
    lastY = y;
    
    yaw += dx * sensitivity;
    pitch += dy * sensitivity;

    if (pitch > 89.0f) pitch = 89.0f;
    if (pitch < -89.0f) pitch = -89.0f;
    
    this->setYawAndPitch(yaw, pitch);
}

inline void FPSCamera::scroll (double dy) {
    float fov = this->fov;
    fov -= (float) dy;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
    Camera::setFov(fov);
}

inline void FPSCamera::move (Direction direction) {
    this->move(direction, 50.0f);
}


inline void FPSCamera::move (Direction direction, float dtime) {
    float y = 0.0f;
    if (!verticalMoveLock) y = front.y;
    switch(direction) {
    case FPSCamera::Direction::FORWARD: 
        Camera::move(normalize(vec3(front.x, y, front.z)) * speed * dtime); 
        break;
    case FPSCamera::Direction::BACKWARD: 
        Camera::move(normalize(vec3(-front.x, -y, -front.z)) * speed * dtime); 
        break;
    case FPSCamera::Direction::LEFT: 
        Camera::move(normalize(cross(up, front)) * speed * dtime); 
        break;
    case FPSCamera::Direction::RIGHT: 
        Camera::move(normalize(cross(front, up)) * speed * dtime); 
        break;
    case FPSCamera::Direction::FORWARD_LEFT: 
        Camera::move(normalize(
            normalize(vec3(front.x, y, front.z)) + 
            normalize(cross(up, front))) * speed * dtime
        );
        break;
    case FPSCamera::Direction::FORWARD_RIGHT: 
        Camera::move(normalize(
            normalize(vec3(front.x, y, front.z)) + 
            normalize(cross(front, up))) * speed * dtime
        );
        break;
    case FPSCamera::Direction::BACKWARD_LEFT: 
        Camera::move(normalize(
            normalize(vec3(-front.x, -y, -front.z)) + 
            normalize(cross(up, front))) * speed * dtime
        );
        break;
    case FPSCamera::Direction::BACKWARD_RIGHT: 
        Camera::move(normalize(
            normalize(vec3(-front.x, -y, -front.z)) + 
            normalize(cross(front, up))) * speed * dtime
        );
        break;
    }
}

#endif