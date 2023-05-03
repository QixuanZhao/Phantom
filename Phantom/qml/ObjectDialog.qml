import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

Dialog {
    property string objectName
    property int objectIndex

    id: objectPropertyDialog
    title: qsTr("Object Properties - %1").arg(objectName)
    anchors.centerIn: Overlay.overlay
    modal: false
    Material.background: Qt.rgba(
                             objectAlbedoColourPicker.currentColour.r,
                             objectAlbedoColourPicker.currentColour.g,
                             objectAlbedoColourPicker.currentColour.b,
                             0.1)

    onObjectNameChanged: {
        objectIndex = -1
        console.log(controller.model.count)
        for (let i = 0; i < controller.model.count; i++) {
            console.log(controller.model.get(i))
            if (controller.model.get(i).name === objectName)
                objectIndex = i
        }
        if (objectIndex === -1) return
        let objectItem = controller.model.get(objectIndex)

        polygonModeComboBox.currentIndex = objectItem.polygonMode
        objectAlbedoColourPicker.currentColour = Qt.rgba(objectItem.colour.r, objectItem.colour.g, objectItem.colour.b, 1.0)
        shininessSlider.value = objectItem.shininess
        metallicSlider.value = objectItem.metallic
        roughnessSlider.value = objectItem.roughness
        distinctionCoefficientSlider.value = objectItem.K
    }

    GridLayout {
        width: parent.width
        columns: 2

        Label { text: qsTr("Polygon Mode") }
        ComboBox {
            id: polygonModeComboBox
            Layout.fillWidth: true
            textRole: "text"
            valueRole: "value"
            model: ListModel {
                ListElement { text: qsTr("Fill"); value: Data.PolygonMode.FILL }
                ListElement { text: qsTr("Line"); value: Data.PolygonMode.LINE }
                ListElement { text: qsTr("Point"); value: Data.PolygonMode.POINT }
            }
            onCurrentValueChanged: {
                let objectName = objectPropertyDialog.objectName
                let objectIndex = objectPropertyDialog.objectIndex
                if (objectName === "" || objectIndex === -1) return
                controller.data.setPolygonMode(objectName, currentValue)
                controller.model.setProperty(objectIndex, "polygonMode", currentIndex)
            }
        }

        Label { text: qsTr("Albedo") }
        ColourPicker {
            id: objectAlbedoColourPicker
            Layout.fillWidth: true
            currentColour: "white"
            onCurrentColourChanged: {
                let objectName = objectPropertyDialog.objectName
                let objectIndex = objectPropertyDialog.objectIndex
                if (objectName === "" || objectIndex === -1) return
                controller.data.setColour(objectName, currentColour)
                controller.model.setProperty(objectIndex, "colour", { "r": currentColour.r, "g": currentColour.g, "b": currentColour.b })
            }
        }

        Label { text: qsTr("Shininess") }
        Slider {
            id: shininessSlider
            Layout.fillWidth: true
            from: 1
            to: 2048
            stepSize: 1
            value: 32
            onValueChanged: {
                let objectName = objectPropertyDialog.objectName
                let objectIndex = objectPropertyDialog.objectIndex
                if (objectName === "" || objectIndex === -1) return
                controller.data.setShininess(objectName, value)
                controller.model.setProperty(objectIndex, "shininess", value)
            }

            ToolTip {
                text: qsTr("Value: %1").arg(shininessSlider.value)
                visible: shininessSlider.enabled && shininessSlider.hovered
                parent: shininessSlider.handle
            }
        }


        Label { text: qsTr("Metallic") }
        Slider {
            id: metallicSlider
            Layout.fillWidth: true
            from: 0
            to: 1
            value: 0.5
            onValueChanged: {
                let objectName = objectPropertyDialog.objectName
                let objectIndex = objectPropertyDialog.objectIndex
                if (objectName === "" || objectIndex === -1) return
                controller.data.setMetallic(objectName, value)
                controller.model.setProperty(objectIndex, "metallic", value)
            }

            ToolTip {
                text: qsTr("Value: %1").arg(metallicSlider.value)
                visible: metallicSlider.enabled && metallicSlider.hovered
                parent: metallicSlider.handle
            }
        }

        Label { text: qsTr("Roughness") }
        Slider {
            id: roughnessSlider
            Layout.fillWidth: true
            from: 0
            to: 1
            value: 0.5
            onValueChanged: {
                let objectName = objectPropertyDialog.objectName
                let objectIndex = objectPropertyDialog.objectIndex
                if (objectName === "" || objectIndex === -1) return
                controller.data.setRoughness(objectName, value)
                controller.model.setProperty(objectIndex, "roughness", value)
            }

            ToolTip {
                text: qsTr("Value: %1").arg(roughnessSlider.value)
                visible: roughnessSlider.enabled && roughnessSlider.hovered
                parent: roughnessSlider.handle
            }
        }

        Label { text: qsTr("Distinction Coefficient") }
        Slider {
            id: distinctionCoefficientSlider
            Layout.fillWidth: true
            from: 0
            to: 20
            value: 0
            onValueChanged: {
                let objectName = objectPropertyDialog.objectName
                let objectIndex = objectPropertyDialog.objectIndex
                if (objectName === "" || objectIndex === -1) return
                controller.data.setK(objectName, value)
                controller.model.setProperty(objectIndex, "K", value)
            }

            ToolTip {
                text: qsTr("Value: %1").arg(distinctionCoefficientSlider.value)
                visible: distinctionCoefficientSlider.enabled && distinctionCoefficientSlider.hovered
                parent: distinctionCoefficientSlider.handle
            }
        }

    }

}
