#include "stdafx.h"

void ObserverCameraController::zoom(const QString& objectName, const QString& surfaceName)
{
	if (objectName.isNull()) {
		Data::getInstance().zoom();
	}
	else {
		QMap<QString, Object3D*>& objectMap = Data::getInstance().getObjects();
		if (objectMap.count(objectName) <= 0) return;

		if (surfaceName.isNull()) {
			objectMap[objectName]->zoom();
		}
		else {
			QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
			if (surfaceMap.count(surfaceName) <= 0) return;

			surfaceMap[surfaceName]->zoom();
		}
	}

	emit cameraPositionChanged();
}