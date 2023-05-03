#pragma once

class MouseCameraController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(qreal pitch READ pitch NOTIFY cameraDirectionChanged)
	Q_PROPERTY(qreal yaw READ yaw NOTIFY cameraDirectionChanged)
	Q_PROPERTY(qreal roll READ roll NOTIFY cameraDirectionChanged)
	Q_PROPERTY(qreal distance READ distance NOTIFY targetDistanceChanged)
	Q_PROPERTY(QVector3D target READ target NOTIFY targetChanged)
	Q_PROPERTY(bool viewLock READ viewLock WRITE setViewLock)

public:
	inline virtual void setCamera(MouseCamera* camera) {
		this->camera = camera;
	}

	inline qreal pitch() const { 
		if (!camera) return 0.0;
		return camera->getPitch(); 
	}
	inline qreal yaw() const {
		if (!camera) return 0.0;
		return camera->getYaw(); 
	}
	inline qreal roll() const {
		if (!camera) return 0.0;
		return camera->getRoll(); 
	}

	inline qreal distance() const {
		if (!camera) return -1.0;
		return camera->getDistance();
	}

	inline QVector3D target() const {
		if (!camera) return QVector3D();
		return QVector3D(camera->getTarget().x, camera->getTarget().y, camera->getTarget().z);
	}

	inline bool viewLock() const {
		if (!camera) return false;
		return camera->isViewLocked();
	}

	inline void setViewLock(bool viewLock) {
		if (!camera) return;
		if (viewLock) camera->lockView();
		else camera->unlockView();
	}

	Q_INVOKABLE inline virtual void setMouse(const QPointF& mouse) {
		if (!camera) return;
		camera->setMouse(mouse.x(), mouse.y());
	}

	Q_INVOKABLE inline virtual void mouse(const QPointF& mouse) {
		if (!camera) return;
		camera->mouse(mouse.x(), mouse.y());
		emit cameraDirectionChanged();
	}

	Q_INVOKABLE inline virtual void wheel(const QPointF& wheel) {
		if (!camera) return;
		camera->scroll(wheel.y());
	}

signals:
	void cameraDirectionChanged();
	void cameraPositionChanged();
	void targetChanged();
	void targetDistanceChanged();

protected:
	MouseCamera* camera = nullptr;
};
