#include "stdafx.h"

QString DataController::load(const QUrl& url)
{
	QString filePath = url.toLocalFile();
	Object3D* object = nullptr;

	if (filePath.endsWith(".obj", Qt::CaseInsensitive)) {
		Surface* surface = new Surface(filePath);
		object = new Object3D(surface->getName());
		object->insertSurface(surface);
	}
	else {
		// tell the idiot user he chose the wrong file
		return QString();
	}

	data.insertObject(object);
	return object->getName();
}

void DataController::remove(const QString& objectName, const QString& surfaceName)
{
	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	if (surfaceName.isNull()) {
		data.deleteObject(objectName);
	}
	else {
		QMap<QString, Surface*>& surfaceMap = object->getSurfaces();
		if (surfaceMap.count(surfaceName) <= 0) return;

		object->deleteSurface(surfaceMap[surfaceName]);
	}
}

void DataController::setVisible(const QString& objectName, const QString& surfaceName, bool visible)
{
	if (surfaceName.isNull()) {
		setVisible(objectName, visible);
		return;
	}

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
	if (surfaceMap.count(surfaceName) <= 0) return;

	surfaceMap[surfaceName]->setVisible(visible);
	Lighting::lighting.updateShadow();
}

void DataController::setVisible(const QString& objectName, bool visible)
{
	if (objectName.isNull()) return;

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	for (Surface* surface : object->getSurfaces()) 
		surface->setVisible(visible);
	Lighting::lighting.updateShadow();
}

void DataController::setColour(const QString& objectName, const QString& surfaceName, const QColor& color)
{
	if (objectName.isNull()) return;
	if (surfaceName.isNull()) {
		setColour(objectName, color);
		return;
	}

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
	if (surfaceMap.count(surfaceName) <= 0) return;

	surfaceMap[surfaceName]->setColour(vec3(color.redF(), color.greenF(), color.blueF()));
}

void DataController::setColour(const QString& objectName, const QColor& color)
{
	if (objectName.isNull()) return;

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	for (Surface* surface : object->getSurfaces())
		surface->setColour(vec3(color.redF(), color.greenF(), color.blueF()));
}

void DataController::setPolygonMode(const QString& objectName, const QString& surfaceName, PolygonMode polygonMode)
{
	if (objectName.isNull()) return;
	if (surfaceName.isNull()) {
		setPolygonMode(objectName, polygonMode);
		return;
	}

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
	if (surfaceMap.count(surfaceName) <= 0) return;

	switch (polygonMode) {
	case PolygonMode::FILL:
		surfaceMap[surfaceName]->setPolygonMode(GL_FILL);
		break;
	case PolygonMode::LINE:
		surfaceMap[surfaceName]->setPolygonMode(GL_LINE);
		break;
	case PolygonMode::POINT:
		surfaceMap[surfaceName]->setPolygonMode(GL_POINT);
		break;
	}

	Lighting::lighting.updateShadow();
}

void DataController::setPolygonMode(const QString& objectName, PolygonMode polygonMode)
{
	if (objectName.isNull()) return;

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	for (Surface* surface : object->getSurfaces()) {
		switch (polygonMode) {
		case PolygonMode::FILL:
			surface->setPolygonMode(GL_FILL);
			break;
		case PolygonMode::LINE:
			surface->setPolygonMode(GL_LINE);
			break;
		case PolygonMode::POINT:
			surface->setPolygonMode(GL_POINT);
			break;
		}
	}

	Lighting::lighting.updateShadow();
}

void DataController::setShininess(const QString& objectName, const QString& surfaceName, qreal shininess)
{
	if (objectName.isNull()) return;
	if (surfaceName.isNull()) {
		setShininess(objectName, shininess);
		return;
	}

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
	if (surfaceMap.count(surfaceName) <= 0) return;

	surfaceMap[surfaceName]->setShininess(shininess);
}

void DataController::setShininess(const QString& objectName, qreal shininess)
{
	if (objectName.isNull()) return;

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	for (Surface* surface : object->getSurfaces())
		surface->setShininess(shininess);
}

void DataController::setMetallic(const QString& objectName, const QString& surfaceName, qreal metallic)
{
	if (objectName.isNull()) return;
	if (surfaceName.isNull()) {
		setMetallic(objectName, metallic);
		return;
	}

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
	if (surfaceMap.count(surfaceName) <= 0) return;

	surfaceMap[surfaceName]->setMetallic(metallic);
}

void DataController::setMetallic(const QString& objectName, qreal metallic)
{
	if (objectName.isNull()) return;

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	for (Surface* surface : object->getSurfaces())
		surface->setMetallic(metallic);
}

void DataController::setRoughness(const QString& objectName, const QString& surfaceName, qreal roughness)
{
	if (objectName.isNull()) return;
	if (surfaceName.isNull()) {
		setRoughness(objectName, roughness);
		return;
	}

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
	if (surfaceMap.count(surfaceName) <= 0) return;

	surfaceMap[surfaceName]->setRoughness(roughness);
}

void DataController::setRoughness(const QString& objectName, qreal roughness)
{
	if (objectName.isNull()) return;

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	for (Surface* surface : object->getSurfaces())
		surface->setRoughness(roughness);
}

void DataController::setK(const QString& objectName, const QString& surfaceName, qreal K)
{
	if (objectName.isNull()) return;
	if (surfaceName.isNull()) {
		setK(objectName, K);
		return;
	}

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	QMap<QString, Surface*>& surfaceMap = objectMap[objectName]->getSurfaces();
	if (surfaceMap.count(surfaceName) <= 0) return;

	surfaceMap[surfaceName]->setK(vec3(float(K)));
}

void DataController::setK(const QString& objectName, qreal K)
{
	if (objectName.isNull()) return;

	QMap<QString, Object3D*>& objectMap = data.getObjects();
	if (objectMap.count(objectName) <= 0) return;

	Object3D* object = objectMap[objectName];

	for (Surface* surface : object->getSurfaces())
		surface->setK(vec3(float(K)));
}

