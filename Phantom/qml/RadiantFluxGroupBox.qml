import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

GroupBox {
    id: groupBox
    title: qsTr("Radiant Flux (%1 W, %2 W, %3 W)").arg(flux.x).arg(flux.y).arg(flux.z)

    readonly property vector3d flux: Qt.vector3d(
                                colour.r,
                                colour.g,
                                colour.b
                            ).times(power)

    property color colour: colourPicker.currentColour
    property real power: powerSlider.value

    onColourChanged: { colourPicker.currentColour = colour }
    onPowerChanged: { powerSlider.value = power }

    GridLayout {
        anchors.fill: parent

        Label {
            Layout.row: 0
            Layout.column: 0
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            text: qsTr("Colour")
        }
        ColourPicker {
            id: colourPicker
            Layout.row: 0
            Layout.column: 1
            Layout.columnSpan: 2
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter

            onCurrentColourChanged: { groupBox.colour = currentColour }
        }

        Label {
            Layout.row: 1
            Layout.column: 0
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignCenter
            text: qsTr("Power")
        }
        Slider {
            id: powerSlider
            Layout.row: 1
            Layout.column: 1
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            orientation: Qt.Horizontal
            from: 0
            to: 1e4
            value: powerSpinBox.value
            stepSize: 1e-2
            onValueChanged: {
                powerSpinBox.value = value
                groupBox.power = value
            }
        }
        RealSpinBox {
            id: powerSpinBox
            Layout.row: 1
            Layout.column: 2
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            from: 0
            to: 1e4
            stepSize: powerSlider.stepSize
            value: 0.3
            onValueChanged: {
                powerSlider.value = value
                groupBox.power = value
            }
        }
    }
}
