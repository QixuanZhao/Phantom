#include "stdafx.h"

void LightingController::setDirectionalLight(const qint8& index)
{
	list<ParallelLight>::iterator it = Lighting::lighting.parallelLights.begin();
	for (qint8 i = 0; i < index; i++, ++it);
	directionalLightController.setLight(&*it);
}

void LightingController::setPointLight(const qint8& index)
{
	list<ShadowCastingPointLight>::iterator it = Lighting::lighting.pointLights.begin();
	for (qint8 i = 0; i < index; i++, ++it);
	pointLightController.setLight(&*it);
}

void LightingController::setSpotlight(const qint8& index)
{
	list<ShadowCastingSpotlight>::iterator it = Lighting::lighting.spotlights.begin();
	for (qint8 i = 0; i < index; i++, ++it);
	spotlightController.setLight(&*it);
}
