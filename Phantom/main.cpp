#include "stdafx.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/static/res/icons/ev_shadow_FILL0_wght400_GRAD0_opsz48.svg"));
    app.overrideCursor();

    Control::instance().retranslate(QLocale::system());

    QQuickWindow::setGraphicsApi(QSGRendererInterface::OpenGL);
    qmlRegisterType<RenderingFramebufferObject>("Phantom", 1, 0, "Renderer");
    
    qmlRegisterType<MouseCameraController>("Phantom", 1, 0, "Camera");
    qmlRegisterType<ObserverCameraController>("Phantom", 1, 0, "ObserverCamera");
    qmlRegisterType<FPSCameraController>("Phantom", 1, 0, "FPSCamera");
    qmlRegisterType<AxesController>("Phantom", 1, 0, "Axes");
    qmlRegisterType<DisplayController>("Phantom", 1, 0, "Display");
    qmlRegisterType<AmbientLightController>("Phantom", 1, 0, "AmbientLight");
    qmlRegisterType<ReflectiveLightController>("Phantom", 1, 0, "ReflectiveLight");
    qmlRegisterType<DirectionalLightController>("Phantom", 1, 0, "DirectionalLight");
    qmlRegisterType<AttenuateLightController>("Phantom", 1, 0, "AttenuateLight");

    qmlRegisterSingletonType<ParallelLightModel>("Phantom", 1, 0, "ParallelLightModel", &ParallelLightModel::getInstance);
    qmlRegisterSingletonType<PointLightModel>("Phantom", 1, 0, "PointLightModel", &PointLightModel::getInstance);
    qmlRegisterSingletonType<SpotlightModel>("Phantom", 1, 0, "SpotlightModel", &SpotlightModel::getInstance);
    qmlRegisterSingletonType<AnalyticalLightTreeModel>("Phantom", 1, 0, "AnalyticalLightTreeModel", &AnalyticalLightTreeModel::getInstance);
    qmlRegisterType<ParallelLightController>("Phantom", 1, 0, "ParallelLight");
    qmlRegisterType<PointLightController>("Phantom", 1, 0, "PointLight");
    qmlRegisterType<ShadowCastingPointLightController>("Phantom", 1, 0, "SPointLight");

    qmlRegisterType<SpotlightController>("Phantom", 1, 0, "Spotlight");
    qmlRegisterType<ShadowController>("Phantom", 1, 0, "Shadow");
    qmlRegisterType<LightingController>("Phantom", 1, 0, "Lighting");
    qmlRegisterType<DataController>("Phantom", 1, 0, "Data");
    qmlRegisterType<DebugController>("Phantom", 1, 0, "Debug");
    qmlRegisterType<GraphicsController>("Phantom", 1, 0, "Graphics");
    qmlRegisterType<Controller>("Phantom", 1, 0, "Controller");

    QQmlApplicationEngine& engine = Control::instance().engine; 
    engine.addImportPath(":/qml/");
    engine.load(":/qml/main.qml");

    return app.exec();
}
