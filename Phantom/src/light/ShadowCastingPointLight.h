#pragma once

#ifndef SHADOW_CASTING_POINT_LIGHT_H
#define SHADOW_CASTING_POINT_LIGHT_H

class ShadowCastingPointLight : public PointLight {
public:
	inline ShadowCastingPointLight(
		const PointLight& pointLight = PointLight()
	) : ReflectiveLight(pointLight),
		PointLight(pointLight)
	{ }

	inline ShadowCastingPointLight(
		const ShadowCastingPointLight& bulb
	) : ReflectiveLight(bulb),
		PointLight(bulb)
	{ }

	inline virtual void setPosition(const vec3& position) override
	{
		PointLight::setPosition(position);
		shadow.setPosition(position);
	}

	inline OmnidirectionalShadow& getShadow() { return shadow; }

	inline const OmnidirectionalShadow& getShadow() const { return shadow; }

protected:
	OmnidirectionalShadow shadow;
};

#endif
