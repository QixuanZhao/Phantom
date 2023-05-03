#pragma once

class SpotlightController : public PointLightController
{
	Q_OBJECT
	Q_PROPERTY(qreal azimuth READ azimuth WRITE setAzimuth)
	Q_PROPERTY(qreal altitude READ altitude WRITE setAltitude)
	Q_PROPERTY(qreal innerCutOff READ innerCutOff WRITE setInnerCutOff)
	Q_PROPERTY(qreal outerCutOff READ outerCutOff WRITE setOuterCutOff)
	QML_ELEMENT
public:
	inline virtual void setLight(Spotlight* light) {
		this->light = light;
		PointLightController::setLight(light);
	}

	inline qreal innerCutOff() const {
		if (!light) return -1;
		return light->getInnerCutOff();
	}

	inline qreal outerCutOff() const {
		if (!light) return -1;
		return light->getOuterCutOff();
	}

	inline void setInnerCutOff(qreal cutOff) {
		if (!light) return;
		light->setInnerCutOff(cutOff);
	}

	inline void setOuterCutOff(qreal cutOff) {
		if (!light) return;
		light->setOuterCutOff(cutOff);
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
	Spotlight* light = nullptr;
};
