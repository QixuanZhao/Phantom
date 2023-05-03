#pragma once

class DirectionalLightController : public ReflectiveLightController
{
	Q_OBJECT
	Q_PROPERTY(qreal azimuth READ azimuth WRITE setAzimuth)
	Q_PROPERTY(qreal altitude READ altitude WRITE setAltitude)
	QML_ELEMENT
public:
	Q_INVOKABLE inline virtual void setLight(DirectionalLight* light) {
		this->light = light;
		ReflectiveLightController::setLight(light);
	}

	inline qreal azimuth() const { 
		if (!light) return -1;
		return light->getAzimuth(); 
	}
	inline void setAzimuth(qreal azimuth) {
		if (!light) return;
		light->setAzimuth(azimuth); 
	}

	inline qreal altitude() const {
		if (!light) return -1000;
		return light->getAltitude(); 
	}
	inline void setAltitude(qreal altitude) {
		if (!light) return;
		light->setAltitude(altitude); 
	}

protected:
	DirectionalLight* light = nullptr;
};
