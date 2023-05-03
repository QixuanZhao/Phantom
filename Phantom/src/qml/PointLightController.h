#pragma once

class PointLightController : public AttenuateLightController
{
	Q_OBJECT
	Q_PROPERTY(QVector3D position READ position WRITE setPosition NOTIFY positionChanged)
	QML_ELEMENT
public:
	inline virtual void setLight(PointLight* light) {
		this->light = light;
		AttenuateLightController::setLight(light);
	}

	inline QVector3D position() const {
		if (!light) return QVector3D();
		vec3 position = light->getPosition();
		return QVector3D(position.x, position.y, position.z);
	}

	inline void setPosition(const QVector3D& position) {
		if (!light) return;
		light->setPosition(vec3(position.x(), position.y(), position.z()));
		emit positionChanged(position);
	}

	Q_INVOKABLE inline void moveTop(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& up = Display::display.camera.getUp();
		const vec3& newPosition = lightPosition + float(speed) * up;
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

	Q_INVOKABLE inline void moveBottom(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& up = Display::display.camera.getUp();
		const vec3& newPosition = lightPosition - float(speed) * up;
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

	Q_INVOKABLE inline void moveLeft(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& right = Display::display.camera.getRight();
		const vec3& newPosition = lightPosition - float(speed) * right;
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

	Q_INVOKABLE inline void moveRight(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& right = Display::display.camera.getRight();
		const vec3& newPosition = lightPosition + float(speed) * right;
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

	Q_INVOKABLE inline void moveTopLeft(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& up = Display::display.camera.getUp();
		const vec3& right = Display::display.camera.getRight();
		const vec3& newPosition = lightPosition + float(speed) * glm::normalize(up - right);
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

	Q_INVOKABLE inline void moveTopRight(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& up = Display::display.camera.getUp();
		const vec3& right = Display::display.camera.getRight();
		const vec3& newPosition = lightPosition + float(speed) * glm::normalize(up + right);
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

	Q_INVOKABLE inline void moveBottomLeft(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& up = Display::display.camera.getUp();
		const vec3& right = Display::display.camera.getRight();
		const vec3& newPosition = lightPosition - float(speed) * glm::normalize(up + right);
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

	Q_INVOKABLE inline void moveBottomRight(const qreal& speed) {
		if (!light) return;
		const vec3& lightPosition = light->getPosition();
		const vec3& up = Display::display.camera.getUp();
		const vec3& right = Display::display.camera.getRight();
		const vec3& newPosition = lightPosition + float(speed) * glm::normalize(right - up);
		light->setPosition(newPosition);
		emit positionChanged(QVector3D(newPosition.x, newPosition.y, newPosition.z));
	}

signals:
	void positionChanged(const QVector3D& position);

protected:
	PointLight* light = nullptr;
};
