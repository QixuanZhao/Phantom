import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

Item {
    id: scrollItem
    implicitWidth: childrenRect.width + 20
    implicitHeight: childrenRect.height + 30

    Component.onCompleted: {
        controller.cameraType = Qt.binding(() => cameraTypeComboBox.currentIndex)

        controller.observerCamera.flashOn = Qt.binding(() => flashSwitch.checked)
        controller.fpsCamera.flashOn = Qt.binding(() => flashSwitch.checked)

        controller.observerCamera.flash.colour = Qt.binding(() => flashRadiantFluxGroupBox.colour)
        controller.observerCamera.flash.power = Qt.binding(() => flashRadiantFluxGroupBox.power)
        controller.observerCamera.flash.specular = Qt.binding(() => flashRadiantFluxGroupBox.flux)
        controller.observerCamera.flash.diffuse = Qt.binding(() => flashRadiantFluxGroupBox.flux)

        controller.observerCamera.flash.innerCutOff = Qt.binding(() => flashCutoffGroupBox.innerCutoff)
        controller.observerCamera.flash.outerCutOff = Qt.binding(() => flashCutoffGroupBox.outerCutoff)

        controller.fpsCamera.flash.colour = Qt.binding(() => flashRadiantFluxGroupBox.colour)
        controller.fpsCamera.flash.power = Qt.binding(() => flashRadiantFluxGroupBox.power)
        controller.fpsCamera.flash.specular = Qt.binding(() => flashRadiantFluxGroupBox.flux)
        controller.fpsCamera.flash.diffuse = Qt.binding(() => flashRadiantFluxGroupBox.flux)

        controller.fpsCamera.flash.innerCutOff = Qt.binding(() => flashCutoffGroupBox.innerCutoff)
        controller.fpsCamera.flash.outerCutOff = Qt.binding(() => flashCutoffGroupBox.outerCutoff)
    }

    ColumnLayout {
        id: panel
        anchors.horizontalCenter: parent.horizontalCenter

        RowLayout {
            Label {
                text: qsTr("Camera Mode")
            }

            ComboBox {
                id: cameraTypeComboBox
                Layout.fillWidth: true
                textRole: "text"
                valueRole: "value"
                model: ListModel {
                    ListElement { text: qsTr("Observer"); value: 0 }
                    ListElement { text: qsTr("FPS"); value: 1 }
                }
                currentIndex: 0
            }
        }

        GroupBox {
            id: flashGroupBox
            title: qsTr("Flashlight")
            Layout.fillWidth: true

            label: Switch {
                id: flashSwitch
                checked: false
                text: qsTr("Flash %1").arg(checked ? qsTr("On") : qsTr("Off"))
            }

            ColumnLayout {
                enabled: flashSwitch.checked

                CutoffGroupBox {
                    id: flashCutoffGroupBox
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }

                RadiantFluxGroupBox {
                    id: flashRadiantFluxGroupBox
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                }
            }
        }
    }
}
