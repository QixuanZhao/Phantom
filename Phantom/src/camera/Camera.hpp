﻿#include "stdafx.h"
#pragma once

#ifndef CAMERA_HPP
#define CAMERA_HPP

class Camera
{
public:
    const float NEAR_PLANE_DISTANCE = 0.7f; 
    const float FAR_PLANE_DISTANCE = 5e4f; 

protected:
    float pitch;
    float yaw;
    float roll;
    
    float fov;
    float aspectRatio;
    
    vec3 position;

    vec3 front;
    vec3 right;
    vec3 up;
    
    mat4 projection;
    mat4 view;

private:
    inline void computeView ();
    inline void computeProjection ();
    inline void computeFront ();
    inline void computeRightAndUp();

public:
    inline Camera(
        const float& pitch = -35.0f, 
        const float& yaw = -135.0f, 
        const float& roll = 0.0f, 
        const vec3& position = vec3(0.0f),
        const float& fov = 45.0f, 
        const float& aspectRatio = 16.0f / 9.0f
    );

    inline Camera(const Camera& camera);

    inline const vec3& getFront() const { return front; }
    inline const vec3& getUp() const { return up; }
    inline const vec3& getRight() const { return right; }

    inline const vec3& getPosition() const { return position; }

    inline const mat4& getProjection() const { return projection; }
    inline const mat4& getView() const { return view; }

    inline virtual float getPitch() const { return pitch; }
    inline virtual float getYaw() const { return yaw; }
    inline virtual float getRoll() const { return roll; }

    inline virtual float getFov() const { return fov; }
    inline virtual float getAspectRatio() const { return aspectRatio; }

    inline virtual void setPitch (const float& pitch);
    inline virtual void setYaw (const float& yaw);
    inline virtual void setYawAndPitch(const float& yaw, const float& pitch);
    inline void setPitchAndYaw(const float& pitch, const float& yaw) { setYawAndPitch(yaw, pitch); }
    inline virtual void setRoll (const float& roll);

    inline virtual void setPosition (const vec3& position);
    inline virtual void setPosition (const float& x, const float& y, const float& z);

    inline virtual void setFov (const float& fov);
    inline virtual void setAspectRatio (const float& aspectRatio);

    inline virtual void move (const vec3& offset);
    inline virtual void move (const float& dx, const float& dy, const float& dz);

    inline virtual void configureShaderProgram(ShaderProgram* sp) const {
        sp->set("view", view);
        sp->set("projection", projection);
    }
};

inline Camera::Camera (
    const float& pitch,
    const float& yaw,
    const float& roll,
    const vec3& cameraPosition,
    const float& fov,
    const float& aspectRatio
) : pitch(pitch), 
    yaw(yaw), 
    roll(roll), 
    position(position), 
    fov(fov), 
    aspectRatio(aspectRatio) 
{
    computeFront();
    computeRightAndUp();
    computeView ();
    computeProjection();
}

inline Camera::Camera(const Camera& camera) : 
    Camera(
        camera.pitch, 
        camera.yaw, 
        camera.roll, 
        camera.position, 
        camera.fov, 
        camera.aspectRatio
    ) 
{ }

inline void Camera::computeView () {
    view = glm::lookAt(position, position + front, up);
}

inline void Camera::computeProjection () {
    projection = glm::perspective(
        glm::radians(fov), 
        aspectRatio, 
        NEAR_PLANE_DISTANCE, 
        FAR_PLANE_DISTANCE
    );
}

inline void Camera::computeFront () {
    front = vec3(
        glm::cos(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(yaw)) * glm::cos(glm::radians(pitch)),
        glm::sin(glm::radians(pitch))
    );
    computeView();
}

inline void Camera::computeRightAndUp () {
    right = glm::normalize(
        glm::cross(
            front, 
            vec3(
                glm::sin(glm::radians(roll)),
                0,
                glm::cos(glm::radians(roll))
            )
        )
    );
    up = glm::cross(right, front);
    computeView();
}

inline void Camera::setPitch (const float& pitch) {
    this->pitch = pitch; 
    computeFront();
    computeRightAndUp();
}

inline void Camera::setYaw (const float& yaw) {
    if (yaw > 360.0f || yaw < 0.0f) {
        int integer = yaw;
        float fraction = yaw - integer;
        this->yaw = (integer % 360) + fraction;

        if (this->yaw < 0.0f) this->yaw += 360.0f;
    }

    computeFront();
    computeRightAndUp();
}


inline void Camera::setYawAndPitch(const float& yaw, const float& pitch) {
    if (yaw > 360.0f || yaw < 0.0f) {
        int integer = yaw;
        float fraction = yaw - integer;
        this->yaw = (integer % 360) + fraction;

        if (this->yaw < 0.0f) this->yaw += 360.0f;
    }
    else this->yaw = yaw;

    this->pitch = pitch; 
    computeFront();
    computeRightAndUp();
}

inline void Camera::setRoll (const float& roll) {
    this->roll = roll; 
    computeRightAndUp();
}

inline void Camera::setPosition (const vec3& position) {
    this->position = position; 
    computeView();
}

inline void Camera::setPosition (const float& x, const float& y, const float& z) {
    setPosition(vec3(x, y, z));
}

inline void Camera::setFov (const float& fov) {
    this->fov = fov;
    computeProjection();
}

inline void Camera::setAspectRatio (const float& aspectRatio) {
    this->aspectRatio = aspectRatio;
    computeProjection();
}

inline void Camera::move(const vec3& offset) {
    setPosition(position + offset);
}

inline void Camera::move(const float& dx, const float& dy, const float& dz) {
    move(vec3(dx, dy, dz));
}


#endif