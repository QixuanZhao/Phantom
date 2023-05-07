#pragma once

#ifndef OBSERVER_CAMERA_H
#define OBSERVER_CAMERA_H

class ObserverCamera : public MouseCamera, public FlashCamera
{
protected:
    bool translationLock = true;

    float distanceFactor;
public:
    ObserverCamera(
        const Camera& camera = Camera(),
        const Spotlight& flash = Spotlight(),
        const float& sensitivity = 0.13f,
        const float& distance = 10.0f,
        const vec3& target = vec3(0.0f)
    );

    ObserverCamera(const ObserverCamera & observerCamera) :
        Camera(observerCamera),
        MouseCamera(
            observerCamera, 
            observerCamera.sensitivity, 
            observerCamera.distance, 
            observerCamera.target
        ),
        FlashCamera(observerCamera),
        distanceFactor(observerCamera.distanceFactor)
    { }

    inline virtual void setDistance(const float& distance) override {
        MouseCamera::setDistance(distance);
        computeDistanceFactor(distance);
        setPosition(target - front * distance);
    }
    inline void setTarget(const vec3& target)
    {
        this->target = target;
        setPosition(target - front * distance);
    }
    inline void setTargetAndDistance(const vec3& target, const float& distance) 
    {
        this->target = target;
        MouseCamera::setDistance(distance);
        computeDistanceFactor(distance);
        setPosition(target - front * distance);
    }
    
    inline void lockTranslation() { this->translationLock = true; }
    inline void unlockTranslation() { this->translationLock = false; }

    inline bool isTranslationLocked() const { return translationLock; }

    inline void computeDistanceFactor(const float& distance) { distanceFactor = distance / 13.78f; }

    virtual void mouse (double x, double y) override;
    virtual void scroll (double dy) override;
};  

#endif