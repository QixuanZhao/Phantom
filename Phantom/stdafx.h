#pragma once

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>
#include <string>
#include <random>
#include <vector>
#include <list>
#include <map>

#include <glm/glm.hpp>
#include <glm/gtc/integer.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <QObject>
#include <QApplication>
#include <QMainWindow>
#include <QOpenGLFunctions_4_5_Core>
#include <QOpenGLFramebufferObject>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickItem>
#include <QQuickFramebufferObject>
#include <QDateTime>
#include <QtQml/qqmlregistration.h>
#include <QPointF>
#include <QSize>
#include <QFile>
#include <QTranslator>
#include <QQuickStyle>
#include <QAbstractItemModel>
#include <QAbstractListModel>
#include <QCursor>

using GLF = QOpenGLFunctions_4_5_Core;

using std::vector;
using std::list;
using std::map;
using std::pair;
using std::string;
using std::wstring;

using glm::quat;

using glm::vec;
using glm::vec2;
using glm::vec3;
using glm::vec4;

using glm::ivec2;
using glm::ivec3;
using glm::ivec4;

using glm::mat2;
using glm::mat3;
using glm::mat4;

using glm::mat2x3;
using glm::mat2x4;
using glm::mat3x2;
using glm::mat4x2;
using glm::mat3x4;
using glm::mat4x3;

#include "utilities/TextureCopier.h"

#include "post-process/blur/Blur.h"
#include "post-process/blur/GaussianBlur.h"
#include "post-process/blur/BoxBlur.h"
#include "post-process/blur/KawaseBlur.h"
#include "post-process/blur/DualBlur.h"

#include "configuration/Control.h"
#include "configuration/Debug.h"
#include "configuration/GLConfiguration.h"

#include "utilities/TextFile.h"
#include "utilities/Converter.hpp"
#include "utilities/MathUtility.hpp"
#include "utilities/ShaderProgram.hpp"
#include "utilities/CoordinateSystem.h"

#include "model/AABB.h"
#include "model/Surface.h"
#include "model/Object3D.h"

#include "deferred/GBuffer.h"

#include "light/ssao/SSAO.h"

#include "light/basic/AmbientLight.hpp"
#include "light/basic/CompositeLight.hpp"
#include "light/basic/RadiometricLight.h"
#include "light/basic/ReflectiveLight.h"

#include "light/analytical/AttenuateLight.h"
#include "light/analytical/DirectionalLight.hpp"
#include "light/analytical/PointLight.hpp"
#include "light/analytical/Spotlight.hpp"


#include "light/shadow/Shadow.h"

#include "light/shadow/UnidirectionalShadow.h"
#include "light/shadow/OrthographicShadow.h"
#include "light/shadow/PerspectiveShadow.h"
#include "light/ParallelLight.h"

#include "light/shadow/OmnidirectionalShadow.h"
#include "light/ShadowCastingPointLight.h"
#include "light/ShadowCastingSpotlight.h"

#include "model/ParallelLightModel.h"
#include "model/PointLightModel.h"
#include "model/SpotlightModel.h"
#include "model/AnalyticalLightTreeModel.h"

#include "camera/Camera.hpp"
#include "camera/FlashCamera.hpp"
#include "camera/MouseCamera.hpp"
#include "camera/FPSCamera.hpp"
#include "camera/ObserverCamera.h"

#include "post-process/bloom/Bloom.h"

#include "configuration/Lighting.h"
#include "configuration/Display.h"
#include "configuration/Data.h"

#include "utilities/TextureCopier.hpp"

#include "post-process/blur/Blur.hpp"
#include "post-process/blur/GaussianBlur.hpp"
#include "post-process/blur/BoxBlur.hpp"
#include "post-process/blur/KawaseBlur.hpp"
#include "post-process/blur/DualBlur.hpp"

#include "MainRenderer.h"
#include "qml/RenderingFramebufferObject.h"
#include "qml/AxesController.h"
#include "qml/DisplayController.h"

#include "qml/AmbientLightController.h"

#include "qml/ReflectiveLightController.h"
#include "qml/DirectionalLightController.h"
#include "qml/ParallelLightController.h"
#include "qml/AttenuateLightController.h"
#include "qml/PointLightController.h"
#include "qml/ShadowCastingPointLightController.h"
#include "qml/SpotlightController.h"

#include "qml/MouseCameraController.h"
#include "qml/ObserverCameraController.h"
#include "qml/FPSCameraController.h"

#include "qml/LightingController.h"
#include "qml/ShadowController.h"
#include "qml/DebugController.h"
#include "qml/GraphicsController.h"

#include "qml/DataController.h"
#include "qml/Controller.h"