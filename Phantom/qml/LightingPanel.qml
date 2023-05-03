import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

Item {
    id: lightingScrollItem
    implicitWidth: childrenRect.width + 20
    implicitHeight: childrenRect.height + 30

    Component.onCompleted: {
        controller.lighting.blinnPhong = Qt.binding(() => blinnPhongSwitch.checked)
        controller.lighting.bilateralReflective = Qt.binding(() => bilaterallyReflectiveSwitch.checked)
        controller.lighting.bloom = Qt.binding(() => bloomSwitch.checked)
        controller.lighting.ambientLight.colour = Qt.binding(() => ambientLightColourPicker.currentColour)
        controller.lighting.ambientLight.ssao = Qt.binding(() => ssaoSwitch.checked)
        controller.lighting.ambientLight.ssaoFilterSize = Qt.binding(() => ssaoFilterSizeSlider.value)
        controller.lighting.ambientLight.ssaoRadiusFollowingCamera = Qt.binding(() => ssaoAutoRadiusSwitch.checked)
        controller.lighting.ambientLight.ssaoRadius = Qt.binding(() => ssaoRadiusSlider.value)
        controller.lighting.ambientLight.ssaoBlurPassCount = Qt.binding(() => ssaoBlurSlider.value)
        controller.lighting.ambientLight.ssaoBlurAlgorithm = Qt.binding(() => ssaoBlurAlgorithmComboBox.currentValue)

        controller.shadow.shadowEnabled = Qt.binding(() => shadowSwitch.checked)
        controller.shadow.linearFilterEnabled = Qt.binding(() => shadowLinearFilterSwitch.checked)
        controller.shadow.filterSize = Qt.binding(() => shadowFilterSizeSlider.value)
        controller.shadow.samplingEnabled = Qt.binding(() => shadowSamplingSwitch.checked)
        controller.shadow.samplingSpread = Qt.binding(() => shadowSamplingSpreadSlider.value)
        controller.shadow.samplingMethod = Qt.binding(() => shadowSamplingMethodComboBox.currentValue)
        controller.shadow.samplingRounded = Qt.binding(() => shadowRoundSamplingSwitch.checked)
        controller.shadow.samplingStratified = Qt.binding(() => shadowStratifiedSamplingSwitch.checked)
        controller.shadow.blurPassCount = Qt.binding(() => shadowBlurSlider.value)
        controller.shadow.blurAlgorithm = Qt.binding(() => shadowBlurAlgorithmComboBox.currentValue)
    }

    Connections {
        target: controller.lighting.pointLight
        onPositionChanged: {
            pointLightXSpinBox.value = position.x
            pointLightYSpinBox.value = position.y
            pointLightZSpinBox.value = position.z
        }
    }

    Column {
        id: lightingPanel
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 6

        ColumnLayout {
            spacing: 6
            anchors.horizontalCenter: parent.horizontalCenter

            Switch {
                id: blinnPhongSwitch
                enabled: !controller.graphics.pbrEnabled
                checked: true
                text: "Blinn-Phong %1".arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
            }

            Switch {
                id: bilaterallyReflectiveSwitch
                checked: true
                text: qsTr("Bilaterally Reflective %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
            }

            Switch {
                id: bloomSwitch
                checked: true
                text: qsTr("Bloom %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
            }

            GroupBox {
                title: qsTr("Global Shadow Settings")
                Layout.fillWidth: true

                label: Switch {
                    id: shadowSwitch
                    checked: true
                    text: qsTr("Shadow %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
                }

                ColumnLayout {
                    width: parent.width
                    enabled: shadowSwitch.checked

                    Switch {
                        id: shadowLinearFilterSwitch
                        checked: false
                        text: qsTr("Linear Magnification Filter %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
                    }

                    RowLayout {
                        width: parent.width

                        Label { text: qsTr("Shadow Blur") }
                        Slider {
                            Layout.fillWidth: true
                            id: shadowBlurSlider
                            from: 0
                            to: 20
                            value: 3
                            stepSize: 1

                            ToolTip {
                                text: qsTr("Passes: %1").arg(shadowBlurSlider.value)
                                visible: shadowBlurSlider.enabled && shadowBlurSlider.hovered
                                parent: shadowBlurSlider.handle
                            }
                        }

                        ComboBox {
                            Layout.fillWidth: true
                            textRole: "text"
                            valueRole: "value"
                            id: shadowBlurAlgorithmComboBox
                            model: blurAlgorithmListModel
                            currentIndex: 3
                        }
                    }


                    RowLayout {
                        width: parent.width

                        Switch {
                            id: shadowSamplingSwitch
                            checked: false
                            text: qsTr("Sampling %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
                        }

                        ComboBox {
                            Layout.fillWidth: true
                            textRole: "text"
                            valueRole: "value"
                            id: shadowSamplingMethodComboBox
                            enabled: shadowSamplingSwitch.checked
                            model: ListModel {
                                ListElement { text: qsTr("Percentage Closer Filtering"); value: 0 }
                                ListElement { text: qsTr("Poisson Disk Sampling"); value: 1 }
                            }
                        }
                    }

                    RowLayout {
                        width: parent.width
                        enabled: shadowSamplingSwitch.checked

                        visible: shadowSamplingMethodComboBox.currentValue === 0
                        Label { text: qsTr("Filter Size") }
                        Slider {
                            Layout.fillWidth: true
                            id: shadowFilterSizeSlider
                            from: 1
                            to: 13
                            value: 3
                            stepSize: 1

                            ToolTip {
                                text: qsTr("Filter Size: %1").arg(shadowFilterSizeSlider.value)
                                visible: shadowFilterSizeSlider.enabled && shadowFilterSizeSlider.hovered
                                parent: shadowFilterSizeSlider.handle
                            }
                        }
                        Switch {
                            id: shadowRoundSamplingSwitch
                            text: qsTr("Rounded")
                        }
                    }

                    RowLayout {
                        width: parent.width
                        enabled: shadowSamplingSwitch.checked
                        visible: shadowSamplingMethodComboBox.currentValue === 1

                        Switch {
                            id: shadowStratifiedSamplingSwitch
                            text: qsTr("Stratified")
                        }
                    }

                    GridLayout {
                        width: parent.width
                        enabled: shadowSamplingSwitch.checked
                        columns: 3
                        rowSpacing: 0

                        Label { text: qsTr("Sampling Spread") }
                        Slider {
                            Layout.fillWidth: true
                            id: shadowSamplingSpreadSlider
                            from: 1
                            to: 50
                            value: 30

                            ToolTip {
                                text: qsTr("Sampling Spread: %1").arg(shadowSamplingSpreadSlider.value)
                                visible: shadowSamplingSpreadSlider.enabled && shadowSamplingSpreadSlider.hovered
                                parent: shadowSamplingSpreadSlider.handle
                            }
                        }
                        Button {
                            text: qsTr("Reset")
                            onClicked: {
                                shadowSamplingSpreadSlider.value = 5
                            }
                        }
                    }
                }
            }

            GroupBox {
                title: qsTr("Ambient Light Settings")
                Layout.fillWidth: true

                ColumnLayout {
                    width: parent.width

                    RowLayout {
                        width: parent.width
                        Label { text: qsTr("Colour") }

                        ColourPicker {
                            id: ambientLightColourPicker
                            currentColour: Qt.rgba(0.1, 0.1, 0.1, 1)
                            Layout.fillWidth: true
                        }
                    }

                    Switch {
                        id: ssaoSwitch
                        text: qsTr("Ambient Occlusion")
                        checked: true
                        enabled: controller.graphics.deferredRendering
                    }

                    GridLayout {
                        width: parent.width
                        rowSpacing: 0

                        enabled: ssaoSwitch.checked && controller.graphics.deferredRendering
                        columns: 3
                        Label { text: qsTr("Filter Size") }
                        Slider {
                            id: ssaoFilterSizeSlider
                            Layout.fillWidth: true
                            Layout.columnSpan: 2
                            from: 1
                            to: 64
                            value: 8
                            stepSize: 1

                            ToolTip {
                                text: qsTr("Filter Size: %1").arg(ssaoFilterSizeSlider.value)
                                visible: ssaoFilterSizeSlider.enabled && ssaoFilterSizeSlider.hovered
                                parent: ssaoFilterSizeSlider.handle
                            }
                        }
                        Label { text: qsTr("Radius") }
                        Slider {
                            id: ssaoRadiusSlider
                            Layout.fillWidth: true
                            from: 0.01
                            to: 1e2
                            value: 30
                            enabled: !ssaoAutoRadiusSwitch.checked

                            ToolTip {
                                text: qsTr("Radius: %1").arg(ssaoRadiusSlider.value)
                                visible: ssaoRadiusSlider.enabled && ssaoRadiusSlider.hovered
                                parent: ssaoRadiusSlider.handle
                            }
                        }
                        Switch {
                            id: ssaoAutoRadiusSwitch
                            checked: false
                            text: qsTr("Follow Camera")
                            onCheckedChanged: {
                                if (!checked)
                                    ssaoRadiusSlider.valueChanged();
                            }
                        }

                        Label { text: qsTr("Signal Blur") }
                        Slider {
                            Layout.fillWidth: true
                            id: ssaoBlurSlider
                            from: 0
                            to: 20
                            value: 3
                            stepSize: 1

                            ToolTip {
                                text: qsTr("Passes: %1").arg(ssaoBlurSlider.value)
                                visible: ssaoBlurSlider.enabled && ssaoBlurSlider.hovered
                                parent: ssaoBlurSlider.handle
                            }
                        }

                        ComboBox {
                            Layout.fillWidth: true
                            textRole: "text"
                            valueRole: "value"
                            id: ssaoBlurAlgorithmComboBox
                            model: blurAlgorithmListModel
                            currentIndex: 3
                        }
                    }
                }
            }
        }
    }
}
