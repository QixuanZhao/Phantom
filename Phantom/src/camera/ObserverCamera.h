#pragma once

/**
 * @file    ObserverCamera.h
 * @brief   This file defines the ObserverCamera class. <br/>
 *          此文件定义 ObserverCamera 类。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
#ifndef OBSERVER_CAMERA_H
#define OBSERVER_CAMERA_H

/**
 * @brief A camera observing a specific point. <br/> 观察一个指定点的相机。
 * @details When eular angle changes, camera position changes too: 
 *          camera always lies on a sphere surface whose center and radius are set as 
 *          target and distance of this camera. <br/>
 *          欧拉角发生变化时，相机位置也发生变化：相机永远在一个以目标点为球心、以距离为半径的球面上。
 * @author  赵启轩 <QixuanZhao@outlook.com>
 */
class ObserverCamera : public MouseCamera, public FlashCamera
{
protected:
    float distance;
    vec3 target;

    bool viewLock = true;
    bool translationLock = true;

    float distanceFactor;

public:
    ObserverCamera(
        const FlashCamera& flashCamera,
        const float& sensitivity = 0.13f,
        const float& distance = 10.0f,
        const vec3& target = vec3(0.0f)
    );

    ObserverCamera(
        const Camera& camera = Camera(),
        const Spotlight& flash = Spotlight(),
        const float& sensitivity = 0.13f,
        const float& distance = 10.0f,
        const vec3& target = vec3(0.0f)
    );

    ObserverCamera(const ObserverCamera & observerCamera) :
        Camera(observerCamera),
        MouseCamera(observerCamera),
        FlashCamera(observerCamera),
        distance(observerCamera.distance),
        target(observerCamera.target),
        distanceFactor(observerCamera.distanceFactor)
    { }

    inline void setDistance(const float& distance) {
        this->distance = distance;
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
        this->distance = distance;
        setPosition(target - front * distance);
    }
    
    inline void lockView () { this->viewLock = true; }
    inline void unlockView () { this->viewLock = false; }
    inline void lockTranslation() { this->translationLock = true; }
    inline void unlockTranslation() { this->translationLock = false; }

    inline float getDistance() const { return distance; }
    inline const vec3 & getTarget() const { return target; }
    inline bool isViewLocked() const { return viewLock; }
    inline bool isTranslationLocked() const { return translationLock; }

    inline void computeDistanceFactor(const float& distance) { distanceFactor = distance / 13.78f; }

    virtual void mouse (double x, double y) override;
    virtual void scroll (double dy) override;
};  

#endif