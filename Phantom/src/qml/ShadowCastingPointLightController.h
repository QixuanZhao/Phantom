#pragma once

class ShadowCastingPointLightController : public PointLightController
{
	Q_OBJECT
	QML_ELEMENT
public:
	inline virtual void setLight(ShadowCastingPointLight* light) {
		this->light = light;
		PointLightController::setLight(light);
	}

protected:
	ShadowCastingPointLight* light = nullptr;
};
