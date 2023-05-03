#pragma once

class FPSCameraController : public MouseCameraController
{
	Q_OBJECT
	Q_PROPERTY(qreal speed READ speed WRITE setSpeed NOTIFY speedChanged)
	Q_PROPERTY(bool flashOn READ flashOn WRITE setFlashOn)
	Q_PROPERTY(SpotlightController* flash READ flash)
	QML_ELEMENT
public:
	inline virtual void setCamera(FPSCamera* camera) {
		this->camera = camera;
		MouseCameraController::setCamera(camera);
		cameraFlashController.setLight(&camera->getFlash());
	}

	inline qreal speed() const { 
		if (!camera) return 0.0;
		return camera->getSpeed(); 
	}

	inline void setSpeed(const qreal& speed) {
		if (!camera) return;
		camera->setSpeed(speed);
		emit speedChanged();
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

	inline FPSCamera * getCamera() { return camera; }

	Q_INVOKABLE inline virtual void mouse(const QPointF& mouse) override {
		if (!camera) return;
		MouseCameraController::mouse(mouse);
		emit targetChanged();
	}

signals:
	void speedChanged();

protected:
	FPSCamera* camera = nullptr;
	SpotlightController cameraFlashController;
};
