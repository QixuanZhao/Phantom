#pragma once

#ifndef DIRECTIONAL_LIGHT_HPP
#define DIRECTIONAL_LIGHT_HPP

class DirectionalLight : virtual public ReflectiveLight {
protected:
    vec3 direction; 

    float azimuth;
    float altitude;
    
    inline void computeDirectionFromHorizontal() {
        float altitudeInRadians = glm::radians(altitude);
        float azimuthInRadians = glm::radians(azimuth);
        setDirection(vec3(
            glm::cos(altitudeInRadians) * glm::cos(azimuthInRadians),
            glm::cos(altitudeInRadians) * glm::sin(azimuthInRadians),
            glm::sin(altitudeInRadians)
        ));
    }

    inline void computeHorizontalFromDirection() {
        float altitudeInRadians = glm::asin(direction.z);
        altitude = glm::degrees(altitudeInRadians);
        float cosineAltitude = glm::cos(altitudeInRadians);
        float sineAzimuth = direction.y / cosineAltitude;
        float cosineAzimuth = direction.x / cosineAltitude;
        if (cosineAzimuth >= 0) { // [-pi/2, pi/2]
            azimuth = glm::degrees(glm::asin(sineAzimuth));
        }
        else if (sineAzimuth >= 0) { // [pi/2, pi] 
            azimuth = glm::degrees(glm::acos(cosineAzimuth));
        }
        else { // [-pi, -pi/2]
            azimuth = glm::degrees(-glm::asin(sineAzimuth)) - 180.0f;
        }
    }

public:
    inline DirectionalLight(
        const ReflectiveLight& l,
        const vec3& direction = vec3(0.0f, 0.0f, 1.0f)
    ) : ReflectiveLight(l),
        direction(direction)
    {
        computeHorizontalFromDirection();
    }

    inline DirectionalLight(
        const ReflectiveLight& l = ReflectiveLight(),
        const float& azimuth = -90.0f,
        const float& altitude = 60.0f
    ) : ReflectiveLight(l),
        azimuth(azimuth),
        altitude(altitude)
    {
        computeDirectionFromHorizontal();
    }

    inline DirectionalLight(
        const DirectionalLight& directionalLight
    ) : ReflectiveLight(directionalLight),
        direction(directionalLight.direction),
        azimuth(directionalLight.azimuth),
        altitude(directionalLight.altitude)
    { }

    inline virtual ~DirectionalLight() { }

    inline const vec3 & getDirection() const { return direction; }
    inline const float& getAzimuth() const { return azimuth; }
    inline const float& getAltitude() const { return altitude; }

    inline virtual void setDirection(const vec3 & direction) {
        this->direction = direction;
        computeHorizontalFromDirection();
    }
    inline virtual void setAzimuth(const float& azimuth) {
        this->azimuth = azimuth;
        computeDirectionFromHorizontal();
    }
    inline virtual void setAltitude(const float& altitude) {
        this->altitude = altitude;
        computeDirectionFromHorizontal();
    }
    inline virtual void setHorizontal(const float& azimuth, const float& altitude) {
        this->azimuth = azimuth;
        this->altitude = altitude;
        computeDirectionFromHorizontal();
    }

    inline virtual void configureShaderProgram(ShaderProgram* sp, const QString& varName) const {
        ReflectiveLight::configureShaderProgram(sp, varName);
        sp->set(QStringLiteral("%1.direction").arg(varName), direction);
    }
};

#endif