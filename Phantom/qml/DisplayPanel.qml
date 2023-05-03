import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

Item {
    id: displayScrollItem
    implicitWidth: childrenRect.width + 20
    implicitHeight: childrenRect.height + 30

    Component.onCompleted: {
        controller.display.backgroundColour = Qt.binding(() => backgroundColourPicker.currentColour)
        controller.display.gamma = Qt.binding(() => gammaSlider.value)
        controller.display.toneMappingEnabled = Qt.binding(() => toneMappingSwitch.checked)
        controller.display.blurPassCount = Qt.binding(() => blurPassCountSlider.value)
        controller.display.blurAlgorithm = Qt.binding(() => blurAlgorithmComboBox.currentValue)
        controller.display.toneMappingMethod = Qt.binding(() => toneMappingMethodComboBox.currentValue)
        controller.display.exposure = Qt.binding(() => exposureSlider.value)

        controller.axes.show = Qt.binding(() => showAxes.checked)
    }

    GridLayout {
        id: displayPanel
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            text: qsTr("Background Colour")
            Layout.row: 0
            Layout.column: 0
            Layout.alignment: Qt.AlignTrailing
        }
        ColourPicker {
            id: backgroundColourPicker
            currentColour: "black"
            Layout.row: 0
            Layout.column: 1
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        Label {
            text: qsTr("Gamma")
            Layout.row: 1
            Layout.column: 0
            Layout.alignment: Qt.AlignTrailing
        }
        Slider {
            id: gammaSlider
            from: 1.0
            to: 3.0
            value: 2.2
            stepSize: 0.1
            Layout.row: 1
            Layout.column: 1
            Layout.columnSpan: 2
            Layout.fillWidth: true

            ToolTip {
                text: "%1".arg(gammaSlider.value)
                visible: gammaSlider.enabled && (gammaSlider.hovered || gammaSlider.pressed)
                parent: gammaSlider.handle
            }
        }

        Label {
            text: qsTr("Blur")
            Layout.row: 2
            Layout.column: 0
            Layout.alignment: Qt.AlignTrailing
        }
        Slider {
            id: blurPassCountSlider
            from: 0
            to: 30
            value: 0
            stepSize: 1
            Layout.row: 2
            Layout.column: 1
            Layout.fillWidth: true

            ToolTip {
                text: qsTr("Passes: %1").arg(blurPassCountSlider.value)
                visible: blurPassCountSlider.enabled && (blurPassCountSlider.hovered || blurPassCountSlider.pressed)
                parent: blurPassCountSlider.handle
            }
        }
        ComboBox {
            Layout.row: 2
            Layout.column: 2
            Layout.fillWidth: true
            textRole: "text"
            valueRole: "value"
            id: blurAlgorithmComboBox
            model: blurAlgorithmListModel
        }

        GridLayout {
            Layout.row: 3
            Layout.column: 0
            Layout.columnSpan: 3

            Switch {
                id: showAxes
                text: qsTr("Show Axes")
                Layout.row: 3
                Layout.column: 0
            }

            Switch {
                id: toneMappingSwitch
                text: qsTr("Tone Mapping")
                checked: true
                Layout.row: 4
                Layout.column: 0
            }

            ComboBox {
                id: toneMappingMethodComboBox
                enabled: toneMappingSwitch.checked
                textRole: "text"
                valueRole: "value"
                model: [
                    { text: qsTr("Reinhard"), value: Display.ToneMappingMethod.Reinhard },
                    { text: qsTr("Exposure"), value: Display.ToneMappingMethod.Exposure }
                ]
                Layout.row: 4
                Layout.column: 1
                Layout.columnSpan: 2
                Layout.fillWidth: true
            }
        }


        Label {
            enabled: exposureSlider.visible
            text: qsTr("Exposure")
            Layout.row: 4
            Layout.column: 0
            Layout.alignment: Qt.AlignTrailing
        }
        Slider {
            id: exposureSlider
            enabled: toneMappingSwitch.checked && toneMappingMethodComboBox.currentValue === Display.ToneMappingMethod.Exposure
            from: 0
            to: 10
            value: 1
            stepSize: 1e-3
            Layout.row: 4
            Layout.column: 1
            Layout.columnSpan: 2
            Layout.fillWidth: true

            ToolTip {
                text: "%1".arg(exposureSlider.value)
                visible: exposureSlider.enabled && (exposureSlider.hovered || exposureSlider.pressed)
                parent: exposureSlider.handle
            }
        }
    }
}
