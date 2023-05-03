#pragma once

class ParallelLightController : public DirectionalLightController
{
	Q_OBJECT
	QML_ELEMENT
public:
	inline virtual void setLight(ParallelLight* light) {
		this->light = light;
		DirectionalLightController::setLight(light);
	}

protected:
	ParallelLight* light = nullptr;
};
