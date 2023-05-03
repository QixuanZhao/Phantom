#pragma once

class ReflectiveLightController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(QVector3D specular READ specular WRITE setSpecular)
	Q_PROPERTY(QVector3D diffuse READ diffuse WRITE setDiffuse)
	Q_PROPERTY(QVector3D flux READ flux)
	Q_PROPERTY(QColor colour READ colour WRITE setColour)
	Q_PROPERTY(qreal power READ power WRITE setPower)
	QML_ELEMENT
public:
	inline virtual void setLight(ReflectiveLight* light) { this->light = light; }

	inline QVector3D diffuse() const { 
		if (!light) return QVector3D();
		vec3 diffuse = light->diffuse();
		return QVector3D(diffuse.r, diffuse.g, diffuse.b);
	}
	inline QVector3D specular() const {
		if (!light) return QVector3D();
		vec3 specular = light->specular();
		return QVector3D(specular.r, specular.g, specular.b);
	}
	inline QVector3D flux() const {
		if (!light) return QVector3D();
		vec3 flux = light->flux();
		return QVector3D(flux.r, flux.g, flux.b);
	}
	inline QColor colour() const {
		if (!light) return QColor();
		vec3 colour = light->colour() * 255.0f;
		return QColor(colour.r, colour.g, colour.b);
	}
	inline qreal power() const {
		if (!light) return 0.0;
		return light->power();
	}
	inline void setDiffuse(const QVector3D& color) {
		if (!light) return;
		vec3 colour = vec3(color.x(), color.y(), color.z());
		light->setDiffuse(colour);
	}
	inline void setSpecular(const QVector3D& color) {
		if (!light) return;
		vec3 colour = vec3(color.x(), color.y(), color.z());
		light->setSpecular(colour);
	}
	inline void setColour(const QColor& color) {
		if (!light) return;
		vec3 colour = vec3(color.redF(), color.greenF(), color.blueF());
		light->setColour(colour);
	}
	inline void setPower(const qreal& power) {
		if (!light) return;
		light->setPower(power);
	}

protected:
	ReflectiveLight* light = nullptr;
};
