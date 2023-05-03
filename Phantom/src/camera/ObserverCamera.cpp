#include "stdafx.h"

ObserverCamera::ObserverCamera(
    const FlashCamera& flashCamera,
    const float& sensitivity,
    const float& distance,
    const vec3& target
) : Camera(flashCamera),
    MouseCamera(flashCamera, sensitivity),
    FlashCamera(flashCamera),
    distance(distance),
    target(target),
    distanceFactor(distance / 13.78f)
{ }

ObserverCamera::ObserverCamera(
    const Camera& camera,
    const Spotlight& flash,
    const float& sensitivity,
    const float& distance,
    const vec3& target
) : Camera(camera),
    MouseCamera(camera, sensitivity),
    FlashCamera(camera, flash),
    distance(distance),
    target(target)
{
    computeDistanceFactor(distance);
}

void ObserverCamera::mouse(double x, double y) {
    float dx = float(x - lastX) * sensitivity;
    float dy = float(lastY - y) * sensitivity;
    lastX = x;
    lastY = y;

    if (!viewLock) {
        yaw -= dx;
        pitch += dy;

        if (pitch > 89.999f) pitch = 89.999f;
        if (pitch < -89.999f) pitch = -89.999f;

        setPitchAndYaw(pitch, yaw);
        setPosition(target - front * distance);
    }
    else if (!translationLock) {
        target -= (getUp() * dy + getRight() * dx) * distanceFactor / 7.67f;
        setPosition(target - front * distance);
    }
}


void ObserverCamera::scroll(double dy) {
    //if (viewLock) return;

    float distance = this->distance;
    computeDistanceFactor(distance);
    distance -= dy * distanceFactor * sensitivity / 7.67f;


    if (distance < NEAR_PLANE_DISTANCE) distance = NEAR_PLANE_DISTANCE;
    if (distance > FAR_PLANE_DISTANCE) distance = FAR_PLANE_DISTANCE;

    setDistance(distance);
}
