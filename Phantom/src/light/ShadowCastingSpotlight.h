#pragma once

#ifndef SHADOW_CASTING_SPOTLIGHT_H
#define SHADOW_CASTING_SPOTLIGHT_H

class ShadowCastingSpotlight : public Spotlight {
public:
	inline ShadowCastingSpotlight(
		const Spotlight& light = Spotlight()
	) : ReflectiveLight(light),
		Spotlight(light)
	{
		shadow.setPosition(position);
		shadow.setDirection(direction);
	}

	inline ShadowCastingSpotlight(
		const ShadowCastingSpotlight& flash
	) : ReflectiveLight(flash),
		Spotlight(flash)
	{
		shadow.setPosition(position);
		shadow.setDirection(direction);
	}

	inline virtual void setPosition(const vec3& position) override
	{
		Spotlight::setPosition(position);
		shadow.setPosition(position);
	}

	inline virtual void setDirection(const vec3& direction) override
	{
		Spotlight::setDirection(direction);
		shadow.setDirection(direction);
	}
	inline virtual void setAzimuth(const float& azimuth) override {
		Spotlight::setAzimuth(azimuth);
		shadow.setDirection(direction);
	}
	inline virtual void setAltitude(const float& altitude) override {
		Spotlight::setAltitude(altitude);
		shadow.setDirection(direction);
	}
	inline virtual void setHorizontal(const float& azimuth, const float& altitude) override {
		Spotlight::setHorizontal(azimuth, altitude);
		shadow.setDirection(direction);
	}

	inline virtual void setOuterCutOff(const float& cutoff) override {
		Spotlight::setOuterCutOff(cutoff);
		shadow.setOuterCutoff(cutoff);
	}

	inline PerspectiveShadow& getShadow() { return shadow; }

	inline const PerspectiveShadow& getShadow() const { return shadow; }

protected:
	PerspectiveShadow shadow;
};

#endif
