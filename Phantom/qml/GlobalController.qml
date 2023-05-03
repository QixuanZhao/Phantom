import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

Controller {
    id: controller

    function load(file: string) {
        let name = data.load(file)
        model.append({
            "name": name,
            "colour": {
                "r": 1.0,
                "g": 1.0,
                "b": 1.0
            },
            "polygonMode": 0,
            "shininess": 32.0,
            "metallic": 0.5,
            "roughness": 0.5,
            "K": 0.0
        })
    }

    function remove(index: int) {
        let name = model.get(index).name
        model.remove(index);
        data.remove(name)
    }

    property ListModel model: ListModel {}

    lighting {
        property ListModel parallelLightSources: ListModel {}
        property ListModel pointLightSources: ListModel {}
        property ListModel spotlights: ListModel {}
    }
}
