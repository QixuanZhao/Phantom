#include "stdafx.h"

Data Data::data;

void Data::computeAABB()
{
	if (objects.empty()) aabb = AABB();
	else {
		bool first = true;
		for (Object3D * object : objects) {
			if (first) {
				first = false;
				aabb = object->getAabb();
			}
			else {
				aabb.min = MathUtility::emin(aabb.min, object->getAabb().min);
				aabb.max = MathUtility::emin(aabb.max, object->getAabb().max);
			}
		}
	}
}

void Data::zoom(CoordinateSystem& axes) const
{
	axes.setLength(getZoomDistance());
}

void Data::zoom(ObserverCamera& camera) const
{
	camera.setTarget(getZoomTarget());
	camera.setDistance(getZoomDistance());
}

void Data::zoom(OrthographicShadow& shadow) const
{
	float distance = getZoomDistance();
	shadow.setFar(distance * 2.0f);
	shadow.setWidthAndHeight(distance, distance);
	shadow.setPosition(getZoomTarget() - distance / 2.0f * shadow.getDirection());
}

void Data::zoom() const
{
	zoom(Display::display.observerCamera);
}

void Data::insertObject(Object3D* object) {
	QString name = object->getName();

	int i = 0;
	while (objects.count(name)) {
		name = QString("%1 (%2)").arg(object->getName()).arg(++i);
	}

	object->setName(name);

	if (objects.empty()) aabb = object->getAabb();
	else {
		aabb.min = MathUtility::emin(aabb.min, object->getAabb().min);
		aabb.max = MathUtility::emax(aabb.max, object->getAabb().max);
	}

	objects.insert(name, object);

	Lighting::lighting.updateShadow();
	for (ParallelLight& pl : Lighting::lighting.parallelLights)
		zoom(pl.getShadow());

	zoom(Display::display.axes);
}