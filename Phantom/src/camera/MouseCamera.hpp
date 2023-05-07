#include "stdafx.h"
#pragma once

#ifndef MOUSE_CAMERA_HPP
#define MOUSE_CAMERA_HPP

class MouseCamera : virtual public Camera {
protected:
    double lastX = 0.0;
    double lastY = 0.0;

    float distance;
    vec3 target;

    float sensitivity;

    bool viewLock = true;
public:
    inline MouseCamera(
        const Camera& camera,
        const float& sensitivity = 1.13f,
        const float& distance = 10.0f,
        const vec3& target = vec3(0.0f)
    ) : Camera(camera),
        sensitivity(sensitivity),
        distance(distance),
        target(target)
    { }

    inline MouseCamera(
        const float& pitch = -35.0f,
        const float& yaw = -135.0f,
        const float& roll = 0.0f,
        const vec3& position = vec3(0.0f),
        const float& fov = 45.0f,
        const float& aspectRatio = 16.0f / 9.0f,
        const float& sensitivity = 0.13f,
        const float& distance = 10.0f,
        const vec3 & target = vec3(0.0f)
    ) : Camera(
            pitch,
            yaw,
            roll,
            position,
            fov,
            aspectRatio
        ), 
        sensitivity(sensitivity),
        distance(distance),
        target(target)
    { }

    inline void setSensitivity(const float& sensitivity) { this->sensitivity = sensitivity; }
    inline const float& getSensitivity() { return sensitivity; }
    
    inline void setMouse(double x, double y) { lastX = x; lastY = y; }
    inline double getLastX() const { return lastX; }
    inline double getLastY() const { return lastY; }

    inline virtual void setDistance(const float& distance) { this->distance = distance; }

    inline float getDistance() const { return distance; }
    inline const vec3& getTarget() const { return target; }

    inline void lockView() { this->viewLock = true; }
    inline void unlockView() { this->viewLock = false; }
    inline bool isViewLocked() const { return viewLock; }

    virtual void mouse(double x, double y) = 0;
    virtual void scroll(double dy) = 0;

};
#endif