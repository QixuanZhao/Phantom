#pragma once

#ifndef DATA_H
#define DATA_H

class Object3D;

class Data
{
private:
	static Data data;

private:
	inline Data() { }

protected:
	QMap<QString, Object3D*> objects;

	AABB aabb;

	QString pickedObjectName = QStringLiteral("");

	void computeAABB();

public:
	inline static Data& instance() { return data; }

	inline QMap<QString, Object3D*>& getObjects() { return objects; }

	inline const QMap<QString, Object3D*>& getObjects() const { return objects; }

	inline const AABB& getAabb() const { return aabb; }
	inline vec3 getZoomTarget() const { return (aabb.max + aabb.min) / 2.0f; }
	inline float getZoomDistance() const { return glm::max(glm::distance(aabb.max, aabb.min), 10.0f); }

	inline void pickObject(const QString& name) {
		pickedObjectName = name;
	}

	inline void releaseObject() {
		pickedObjectName = QStringLiteral("");
	}

	inline Object3D* pickedObject() const {
		if (objects.contains(pickedObjectName)) return objects[pickedObjectName];
		else return nullptr;
	}

	void zoom(CoordinateSystem& axes) const;
	void zoom(ObserverCamera& camera) const;
	void zoom(OrthographicShadow& shadow) const;
	void zoom() const;

	void insertObject(Object3D* object);

	inline void deleteObject(Object3D* object) {
		deleteObject(object->getName());
	}

	inline void deleteObject(const QString& objectName) {
		delete objects.take(objectName);

		computeAABB();

		Lighting::lighting.updateShadow();
		for (ParallelLight& pl : Lighting::lighting.parallelLights) 
			zoom(pl.getShadow());
		
		zoom(Display::display.axes);
	}

	inline void clear() {
		for (Object3D* object : objects)
			delete object;

		objects.clear();

		computeAABB();

		Lighting::lighting.updateShadow();
	}
};

#endif