#pragma once

class Controller : public QQuickItem
{
	Q_OBJECT
	Q_PROPERTY(ObserverCameraController* camera READ getCameraController)
	Q_PROPERTY(AxesController* axes READ getAxesController)
	Q_PROPERTY(DisplayController* display READ getDisplayController)
	Q_PROPERTY(LightingController* lighting READ getLightingController)
	Q_PROPERTY(DataController* data READ getDataController)
	Q_PROPERTY(ShadowController* shadow READ getShadowController)
	Q_PROPERTY(DebugController* debug READ getDebugController)
	Q_PROPERTY(GraphicsController* graphics READ getGraphicsController)
	QML_ELEMENT
public:
	inline ObserverCameraController* getCameraController() { return &cameraController; }
	inline AxesController* getAxesController() { return &axesController; }
	inline DisplayController* getDisplayController() { return &displayController; }
	inline LightingController* getLightingController() { return &lightingController; }
	inline DataController* getDataController() { return &dataController; }
	inline ShadowController* getShadowController() { return &shadowController; }
	inline DebugController* getDebugController() { return &debugController; }
	inline GraphicsController* getGraphicsController() { return &graphicsController; }
	Q_INVOKABLE void reloadShaders() const { GLConfiguration::configuration.updated = false; }
protected:
	ObserverCameraController cameraController;
	AxesController axesController;
	DisplayController displayController;
	LightingController lightingController;
	DataController dataController;
	ShadowController shadowController;
	DebugController debugController;
	GraphicsController graphicsController;
};
