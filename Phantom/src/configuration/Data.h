#pragma once

#ifndef DATA_H
#define DATA_H

class Object3D;

class Data
{
private:
	static Data instance;

private:
	inline Data() { }

protected:
	QMap<QString, Object3D*> objects;

	AABB aabb;

	void computeAABB();

public:
	inline static Data& getInstance() { return instance; }

	inline QMap<QString, Object3D*>& getObjects() { return objects; }

	inline const QMap<QString, Object3D*>& getObjects() const { return objects; }

	inline const AABB& getAabb() const { return aabb; }
	inline vec3 getZoomTarget() const { return (aabb.max + aabb.min) / 2.0f; }
	inline float getZoomDistance() const { return glm::max(glm::distance(aabb.max, aabb.min), 10.0f); }

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