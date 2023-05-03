#include "stdafx.h"

void Object3D::insertSurface(Surface* surface) {
	QString name = surface->getName();

	int i = 0;
	while (surfaces.count(name))
		name = QString("%1 (%2)").arg(surface->getName()).arg(++i);

	surface->setName(name);

	surfaces.insert(name, surface);

	if (surfaces.size() == 1) aabb = surface->getAABB();
	else {
		aabb.max = MathUtility::emax(surface->getAABB().max, aabb.max);
		aabb.min = MathUtility::emin(surface->getAABB().min, aabb.min);
	}
}

void Object3D::deleteSurface(Surface* surface) {
	delete surfaces.take(surface->getName());
	computeAABB();
}

void Object3D::computeAABB() {
	if (surfaces.empty()) {
		aabb.max = aabb.min = vec3(0.0f);
	}
	else {
		bool first = true;
		for (Surface* surface: surfaces) {
			if (first) {
				first = false;
				aabb = surface->getAABB();
				continue;
			}

			aabb.max = MathUtility::emax(surface->getAABB().max, aabb.max);
			aabb.min = MathUtility::emin(surface->getAABB().min, aabb.min);
		}
	}

}

void Object3D::zoom (ObserverCamera& camera) const {
	camera.setTarget(getZoomTarget());
	camera.setDistance(getZoomDistance());
}

void Object3D::zoom() const
{
	zoom(Display::display.observerCamera);
}
