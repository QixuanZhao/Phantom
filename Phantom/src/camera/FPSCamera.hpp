#include "stdafx.h"
#pragma once

#ifndef FPS_CAMERA_HPP
#define FPS_CAMERA_HPP

using glm::normalize;
using glm::cross;

class FPSCamera : public MouseCamera, public FlashCamera {
protected:
    bool verticalMoveLock = false;

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
        const Camera& camera = Camera(),
        const Spotlight& flash = Spotlight(),
        const float& sensitivity = 0.03f,
        const float& speed = 2.5f,
        const float& distance = 10.0f
    ) : Camera(camera),
        MouseCamera(camera, sensitivity, distance),
        FlashCamera(camera, flash),
        speed(speed)
    {
        target = distance * front + position;
    }
    
    inline FPSCamera(const FPSCamera & fpsCamera) :
        Camera(fpsCamera),
        MouseCamera(
            fpsCamera,
            fpsCamera.sensitivity,
            fpsCamera.distance,
            fpsCamera.target
        ),
        FlashCamera(fpsCamera),
        speed(fpsCamera.speed)
    { }

    inline float getSpeed() const { return speed; }
    inline void setSpeed(const float& speed) { this->speed = speed; }

    inline virtual void setPitch(const float& pitch) override {
        MouseCamera::setYaw(yaw);
        FlashCamera::setYaw(yaw);
        target = distance * front + position;
    }

    inline virtual void setYaw(const float& yaw) override {
        MouseCamera::setYaw(yaw);
        FlashCamera::setYaw(yaw);
        target = distance * front + position;
    }

    inline virtual void setYawAndPitch(const float& yaw, const float& pitch) override {
        MouseCamera::setYawAndPitch(yaw, pitch);
        FlashCamera::setYawAndPitch(yaw, pitch);
        target = distance * front + position;
    }

    inline virtual void setPosition(const vec3& position) override {
        MouseCamera::setPosition(position);
        FlashCamera::setPosition(position);
        target = distance * front + position;
    }

    inline virtual void setDistance(const float& distance) override { 
        MouseCamera::setDistance(distance);
        target = distance * front + position;
    }

    inline void lockVerticalMove() { verticalMoveLock = true; }
    inline void unlockVerticalMove() { verticalMoveLock = false; }
    inline bool isVerticalMoveLocked() const { return verticalMoveLock; }

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

    if (viewLock) return;

    float dx = float(x - lastX);
    float dy = float(lastY - y);
    lastX = x;
    lastY = y;
    
    yaw -= dx * sensitivity;
    pitch += dy * sensitivity;

    if (pitch > 89.999f) pitch = 89.999f;
    if (pitch < -89.999f) pitch = -89.999f;
    
    this->setYawAndPitch(yaw, pitch);
}

inline void FPSCamera::scroll (double dy) {
    float fov = this->fov;
    fov -= (float) dy * sensitivity;
    if (fov < 1.0f) fov = 1.0f;
    if (fov > 45.0f) fov = 45.0f;
    Camera::setFov(fov);
}

inline void FPSCamera::move (Direction direction) {
    this->move(direction, 50.0f);
}


inline void FPSCamera::move (Direction direction, float dtime) {
    float z = 0.0f;
    if (!verticalMoveLock) z = front.z;
    switch(direction) {
    case FPSCamera::Direction::FORWARD: 
        Camera::move(normalize(vec3(front.x, front.y, z)) * speed * dtime);
        break;
    case FPSCamera::Direction::BACKWARD: 
        Camera::move(normalize(vec3(-front.x, -front.y, -z)) * speed * dtime);
        break;
    case FPSCamera::Direction::LEFT: 
        Camera::move(-right * speed * dtime);
        break;
    case FPSCamera::Direction::RIGHT: 
        Camera::move(right * speed * dtime); 
        break;
    case FPSCamera::Direction::FORWARD_LEFT: 
        Camera::move(normalize(
            normalize(vec3(front.x, front.y, z)) +
            normalize(-right)) * speed * dtime
        );
        break;
    case FPSCamera::Direction::FORWARD_RIGHT: 
        Camera::move(normalize(
            normalize(vec3(front.x, front.y, z)) +
            normalize(right)) * speed * dtime
        );
        break;
    case FPSCamera::Direction::BACKWARD_LEFT: 
        Camera::move(normalize(
            normalize(vec3(-front.x, -front.y, -z)) +
            normalize(-right)) * speed * dtime
        );
        break;
    case FPSCamera::Direction::BACKWARD_RIGHT: 
        Camera::move(normalize(
            normalize(vec3(-front.x, -front.y, -z)) +
            normalize(right)) * speed * dtime
        );
        break;
    }
}

#endif