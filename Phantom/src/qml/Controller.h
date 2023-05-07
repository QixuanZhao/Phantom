#pragma once

class Controller : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(MouseCameraController* camera READ getCameraController NOTIFY cameraTypeChanged)
	Q_PROPERTY(ObserverCameraController* observerCamera READ getObserverCameraController)
	Q_PROPERTY(FPSCameraController* fpsCamera READ getFPSCameraController)
	Q_PROPERTY(AxesController* axes READ getAxesController)
	Q_PROPERTY(DisplayController* display READ getDisplayController)
	Q_PROPERTY(LightingController* lighting READ getLightingController)
	Q_PROPERTY(DataController* data READ getDataController)
	Q_PROPERTY(ShadowController* shadow READ getShadowController)
	Q_PROPERTY(DebugController* debug READ getDebugController)
	Q_PROPERTY(GraphicsController* graphics READ getGraphicsController)
	Q_PROPERTY(CameraType cameraType READ cameraType WRITE setCameraType NOTIFY cameraTypeChanged)
	QML_ELEMENT
public:
	enum class CameraType {
		Observer = 0,
		FPS
	};

	Q_ENUM(CameraType)

	inline Controller() {
		observerCameraController.setCamera(&Display::display.observerCamera);
		fpsCameraController.setCamera(&Display::display.fpsCamera);
	}

	inline MouseCameraController* getCameraController() { 
		return Display::display.observerMode ? (MouseCameraController*)&observerCameraController : (MouseCameraController*)&fpsCameraController;
	}

	inline ObserverCameraController* getObserverCameraController() {
		return &observerCameraController;
	}

	inline FPSCameraController* getFPSCameraController() {
		return &fpsCameraController;
	}

	inline AxesController* getAxesController() { return &axesController; }
	inline DisplayController* getDisplayController() { return &displayController; }
	inline LightingController* getLightingController() { return &lightingController; }
	inline DataController* getDataController() { return &dataController; }
	inline ShadowController* getShadowController() { return &shadowController; }
	inline DebugController* getDebugController() { return &debugController; }
	inline GraphicsController* getGraphicsController() { return &graphicsController; }
	Q_INVOKABLE void reloadShaders() const { GLConfiguration::configuration.updated = false; }

	inline CameraType cameraType() const { return Display::display.observerMode ? CameraType::Observer : CameraType::FPS; }
	inline void setCameraType(const CameraType& type) {
		switch (type) {
		case CameraType::Observer:
			if (Display::display.observerMode) return;
			Display::display.observerMode = true;
			Display::display.observerCamera.setPitchAndYaw(Display::display.fpsCamera.getPitch(), Display::display.fpsCamera.getYaw());
			Display::display.observerCamera.setTargetAndDistance(Display::display.fpsCamera.getTarget(), Display::display.fpsCamera.getDistance());
			break;
		case CameraType::FPS:
			if (!Display::display.observerMode) return;
			Display::display.observerMode = false;
			Display::display.fpsCamera.setPitchAndYaw(Display::display.observerCamera.getPitch(), Display::display.observerCamera.getYaw());
			Display::display.fpsCamera.setPosition(Display::display.observerCamera.getPosition());
			break;
		default: return;
		}
		emit cameraTypeChanged();
	}

	Q_INVOKABLE void controlCursor(const QPoint& pos);
	Q_INVOKABLE void grabCursor();
	Q_INVOKABLE void releaseCursor();

	Q_INVOKABLE void pressKey(Qt::Key key);
	Q_INVOKABLE void releaseKey(Qt::Key key);

	Q_INVOKABLE void pressButton(Qt::MouseButton button);
	Q_INVOKABLE void releaseButton(Qt::MouseButton button);
	
	Q_INVOKABLE void setMouseCenter(const QPoint& center);

	Q_INVOKABLE inline void retranslate(const QLocale& locale) {
		Control::instance().retranslate(locale);
	}

	Q_INVOKABLE inline void reload() {
		Control::instance().engine.load(":/qml/main.qml");
	}
signals:
	void cameraTypeChanged();

protected:
	ObserverCameraController observerCameraController;
	FPSCameraController fpsCameraController;

	AxesController axesController;
	DisplayController displayController;
	LightingController lightingController;
	DataController dataController;
	ShadowController shadowController;
	DebugController debugController;
	GraphicsController graphicsController;
};
