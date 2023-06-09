#pragma once

#ifndef OBJECT_3D_H
#define OBJECT_3D_H

class Display;
class ObserverCamera;

class Object3D
{
protected:
	QMap<QString, Surface*> surfaces;
	QString name;
	AABB aabb;

	vec3 translation = vec3(0.0f);
	vec3 scale = vec3(1.0f);
	quat rotation = quat(1.0f, 0.0f, 0.0f, 0.0f);

	bool faceCullingEnabled = false;

	GLenum culledFace = GL_BACK;

	GLenum polygonMode = GL_FILL;

public:
	inline Object3D(const QString& name) : name(name), aabb() 
	{ }

	inline ~Object3D() { 
		for (Surface* surface : surfaces)
			delete surface;
	}
	
	void insertSurface(Surface* surface);
	void deleteSurface(Surface* surface);

	void computeAABB();
	inline const AABB& getAabb() const { return aabb; }

	inline QMap<QString, Surface*>& getSurfaces() { return surfaces; }
	inline const QMap<QString, Surface*>& getSurfaces() const { return surfaces; }

	inline const QString& getName() const { return name; }
	inline const void setName(const QString& name) { this->name = name; }

	inline vec3 getZoomTarget() const { return translation + center(); }
	inline float getZoomDistance() const { return glm::length(aabb.max - aabb.min); }

	inline vec3 center() const { return (aabb.max + aabb.min) / 2.0f; }

	void zoom(ObserverCamera& camera) const;
	void zoom() const;

	inline void setFaceCullingEnabled(bool enabled) { this->faceCullingEnabled = enabled; }
	inline bool isFaceCullingEnabled() const { return faceCullingEnabled; }

	inline void setCulledFace(GLenum culledFace) { this->culledFace = culledFace; }
	inline GLenum getCulledFace() const { return culledFace; }

	inline const vec3& getTranslation() const { return translation; }
	inline bool setTranslation(const vec3& translation) {
		if (this->translation == translation) 
			return false;

		this->translation = translation;
		for (Surface* surface : surfaces)
			surface->setTranslation(translation);

		return true;
	}

	inline const quat& getRotation() const { return rotation; }
	inline void setRotation(const quat& rotation) {
		this->rotation = rotation;
		for (Surface* surface : surfaces)
			surface->setRotation(rotation);
	}

	inline void applyRotation(const quat& rotation) {
		this->rotation *= rotation;
		for (Surface* surface : surfaces)
			surface->setRotation(this->rotation);
	}

	inline void draw(ShaderProgram& sp) {
		for (Surface* surface : surfaces) {
			surface->setTranslation(translation);
			surface->draw(sp);
		}
	}

	inline void draw(ShaderProgram& sp, int id) {
		for (Surface* surface : surfaces) {
			surface->setTranslation(translation);
			surface->draw(sp, id);
		}
	}
};

#endif
