#pragma once

class ObserverCameraController : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(qreal pitch READ pitch NOTIFY cameraDirectionChanged)
	Q_PROPERTY(qreal yaw READ yaw NOTIFY cameraDirectionChanged)
	Q_PROPERTY(qreal roll READ roll NOTIFY cameraDirectionChanged)
	Q_PROPERTY(qreal distance READ distance NOTIFY cameraPositionChanged)
	Q_PROPERTY(QVector3D target READ target NOTIFY cameraPositionChanged)
	Q_PROPERTY(bool viewLock READ viewLock WRITE setViewLock)
	Q_PROPERTY(bool translationLock READ translationLock WRITE setTranslationLock)
	Q_PROPERTY(bool flashOn READ flashOn WRITE setFlashOn)
	Q_PROPERTY(SpotlightController* flash READ flash)
	QML_ELEMENT
public:
	inline ObserverCameraController() {
		cameraFlashController.setLight(&camera.getFlash());
	}

	inline qreal pitch() const { return camera.getPitch(); }
	inline qreal yaw() const { return camera.getYaw(); }
	inline qreal roll() const { return camera.getRoll(); }

	inline qreal distance() const { return camera.getDistance(); }
	inline QVector3D target() const { return QVector3D(camera.getTarget().x, camera.getTarget().y, camera.getTarget().z); }

	inline bool viewLock() const { return camera.isViewLocked(); }
	inline bool translationLock() const { return camera.isTranslationLocked(); }

	inline void setViewLock(bool viewLock) {
		if (viewLock) camera.lockView();
		else camera.unlockView();
	}

	inline void setTranslationLock(bool translationLock) {
		if (translationLock) camera.lockTranslation();
		else camera.unlockTranslation();
	}

	inline bool flashOn() const { return camera.isFlashOn(); }
	inline void setFlashOn(bool on) { camera.setFlashOn(on); }

	inline SpotlightController* flash() { return &cameraFlashController; }

	inline ObserverCamera& getCamera() { return camera; }

	Q_INVOKABLE void zoom(const QString& objectName = QString(), const QString& surfaceName = QString());

	Q_INVOKABLE inline void setMouse(const QPointF& mouse) {
		camera.setMouse(mouse.x(), mouse.y());
	}

	Q_INVOKABLE inline void mouse(const QPointF& mouse) {
		camera.mouse(mouse.x(), mouse.y());
		if (!camera.isViewLocked()) emit cameraDirectionChanged();
		if (!camera.isTranslationLocked()) emit cameraPositionChanged();
	}

	Q_INVOKABLE inline void wheel(const QPointF& wheel) {
		camera.scroll(wheel.y());
		emit cameraPositionChanged();
	}

signals:
	void cameraDirectionChanged();
	void cameraPositionChanged();

protected:
	ObserverCamera& camera = Display::display.camera;
	SpotlightController cameraFlashController;
};
