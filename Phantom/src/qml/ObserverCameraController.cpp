#include "stdafx.h"

void ObserverCameraController::zoom(const QString& objectName, const QString& surfaceName)
{
	if (!camera) return;

	if (objectName.isNull()) {
		Data::instance().zoom(*camera);
	}
	else {
		QMap<QString, Object3D*>& objectMap = Data::instance().getObjects();
		if (objectMap.count(objectName) <= 0) return;

		if (surfaceName.isNull()) {
			objectMap[objectName]->zoom(*camera);
		}
		else {
			QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
			if (surfaceMap.count(surfaceName) <= 0) return;

			surfaceMap[surfaceName]->zoom(*camera);
		}
	}

	emit cameraPositionChanged();
}