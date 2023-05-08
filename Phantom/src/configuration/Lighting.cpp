#include "stdafx.h"

Lighting Lighting::lighting;

Object3D* Lighting::lightSourceSphere;

void Lighting::updateShadow()
{
	for (ParallelLight& pl : parallelLights) {
		OrthographicShadow& shadow = pl.getShadow();
		shadow.setPosition(
			Data::instance().getZoomTarget()
			- shadow.getDirection() * Data::instance().getZoomDistance() / 1.7f
		);

		shadow.setWidthAndHeight(
			Data::instance().getZoomDistance(),
			Data::instance().getZoomDistance()
		);

		shadow.requireUpdate();
	}

	for (ShadowCastingPointLight& sl : pointLights) {
		sl.getShadow().requireUpdate();
	}

	for (ShadowCastingSpotlight& ss : spotlights) {
		ss.getShadow().requireUpdate();
	}

	//pointLight.getShadow().requireUpdate();
}
