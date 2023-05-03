#pragma once

class ObserverCameraController : public MouseCameraController
{
	Q_OBJECT
	Q_PROPERTY(bool translationLock READ translationLock WRITE setTranslationLock)
	Q_PROPERTY(bool flashOn READ flashOn WRITE setFlashOn)
	Q_PROPERTY(SpotlightController* flash READ flash)
	QML_ELEMENT
public:
	inline virtual void setCamera(ObserverCamera* camera) {
		this->camera = camera;
		MouseCameraController::setCamera(camera);
		cameraFlashController.setLight(&camera->getFlash());
	}

	inline bool translationLock() const {
		if (!camera) return false;
		return camera->isTranslationLocked(); 
	}

	inline void setTranslationLock(bool translationLock) {
		if (!camera) return;
		if (translationLock) camera->lockTranslation();
		else camera->unlockTranslation();
	}

	inline bool flashOn() const {
		if (!camera) return false;
		return camera->isFlashOn(); 
	}

	inline void setFlashOn(bool on) {
		if (!camera) return;
		camera->setFlashOn(on); 
	}

	inline SpotlightController* flash() { return &cameraFlashController; }

	inline ObserverCamera * getCamera() { return camera; }

	Q_INVOKABLE void zoom(const QString& objectName = QString(), const QString& surfaceName = QString());

	Q_INVOKABLE inline void mouse(const QPointF& mouse) override {
		if (!camera) return;
		camera->mouse(mouse.x(), mouse.y());
		if (!camera->isViewLocked()) emit cameraDirectionChanged();
		if (!camera->isTranslationLocked()) {
			emit cameraPositionChanged();
			emit targetChanged();
		}
	}

	Q_INVOKABLE inline void wheel(const QPointF& wheel) override {
		if (!camera) return;
		MouseCameraController::wheel(wheel);
		emit cameraPositionChanged();
		emit targetDistanceChanged();
	}

protected:
	ObserverCamera* camera = nullptr;
	SpotlightController cameraFlashController;
};
