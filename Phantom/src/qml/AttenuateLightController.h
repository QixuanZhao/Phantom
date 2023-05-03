#pragma once

class AttenuateLightController : public ReflectiveLightController
{
	Q_OBJECT
	Q_PROPERTY(qreal constant READ constant WRITE setConstant)
	Q_PROPERTY(qreal linear READ linear WRITE setLinear)
	Q_PROPERTY(qreal quadratic READ quadratic WRITE setQuadratic)
	QML_ELEMENT
public:
	inline virtual void setLight(AttenuateLight* light) { 
		this->light = light;
		ReflectiveLightController::setLight(light);
	}

	inline qreal constant() const {
		if (!light) return 0;
		return light->getConstant();
	}

	inline qreal linear() const {
		if (!light) return 0;
		return light->getLinear();
	}

	inline qreal quadratic() const {
		if (!light) return 0;
		return light->getQuadratic();
	}

	inline void setConstant(qreal constant) {
		if (!light) return;
		light->setConstant(constant);
	}

	inline void setLinear(qreal linear) {
		if (!light) return;
		light->setLinear(linear);
	}

	inline void setQuadratic(qreal quadratic) {
		if (!light) return;
		light->setQuadratic(quadratic);
	}

protected:
	AttenuateLight* light = nullptr;
};
