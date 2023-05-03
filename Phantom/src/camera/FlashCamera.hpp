#pragma once

#ifndef FLASH_CAMERA_HPP
#define FLASH_CAMERA_HPP

class FlashCamera : virtual public Camera {
protected:
    Spotlight flash;
    bool flashOn;

    inline void updateFlashAttitude() {
        this->flash.setPosition(position);
        this->flash.setDirection(front);
    }

public:
    inline FlashCamera(
        const Camera & camera,
        const Spotlight & flash = Spotlight()
    ) : Camera(camera),
        flash(flash),
        flashOn(false)
    { 
        updateFlashAttitude();
    }

    inline FlashCamera(
        const float& pitch = -35.0f,
        const float& yaw = -135.0f,
        const float& roll = 0.0f,
        const vec3 & position = vec3(0.0f),
        const float& fov = 45.0f,
        const float& aspectRatio = 16.0f / 9.0f,
        const Spotlight & flash = Spotlight()
    ) : FlashCamera(
            Camera(
                pitch,
                yaw,
                roll,
                position,
                fov,
                aspectRatio
            ),
            flash
        ) 
    { }

    inline FlashCamera(const FlashCamera & camera) :
        FlashCamera(camera, camera.flash)
    { }

    inline virtual void turnOnFlash () { flashOn = true; }
    inline virtual void turnOffFlash () { flashOn = false; }

    inline virtual bool isFlashOn () { return flashOn; }

    inline virtual Spotlight& getFlash () { return flash; }
    inline void setFlash(const Spotlight& flash) { 
        this->flash = flash;
        updateFlashAttitude();
    }

    inline virtual void setFlashOn(bool on) { flashOn = on; }

    inline virtual void setFlashOuterCutOff (const float& outerCutOff) { flash.setOuterCutOff(outerCutOff); }
    inline virtual void setFlashInnerCutOff (const float& innerCutOff) { flash.setInnerCutOff(innerCutOff); }

    inline virtual void setPosition (const vec3& position) override {
        Camera::setPosition(position); 
        flash.setPosition(position);
    }

    inline virtual void setPitch(const float& pitch) override {
        Camera::setPitch(pitch);
        flash.setDirection(front);
    }

    inline virtual void setYaw(const float& yaw) override {
        Camera::setYaw(yaw);
        flash.setDirection(front);
    }

    inline virtual void setYawAndPitch(const float& yaw, const float& pitch) override {
        Camera::setYawAndPitch(yaw, pitch);
        flash.setDirection(front);
    }
};

#endif