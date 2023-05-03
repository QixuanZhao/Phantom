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

        controller.camera.flashOn = Qt.binding(() => spotlightSwitch.checked)

        controller.camera.flash.colour = Qt.binding(() => spotlightRadiantFluxGroupBox.colour)
        controller.camera.flash.power = Qt.binding(() => spotlightRadiantFluxGroupBox.power)
        controller.camera.flash.specular = Qt.binding(() => spotlightRadiantFluxGroupBox.flux)
        controller.camera.flash.diffuse = Qt.binding(() => spotlightRadiantFluxGroupBox.flux)

        controller.camera.flash.constant = Qt.binding(() => spotlightConstantSlider.value)
        controller.camera.flash.linear = Qt.binding(() => spotlightLinearSlider.value)
        controller.camera.flash.quadratic = Qt.binding(() => spotlightQuadraticSlider.value)
        controller.camera.flash.innerCutOff = Qt.binding(() => spotlightCutoffGroupBox.innerCutoff)
        controller.camera.flash.outerCutOff = Qt.binding(() => spotlightCutoffGroupBox.outerCutoff)

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

            GroupBox {
                title: qsTr("Global Lighting Settings")
                Layout.fillWidth: true

                GridLayout {
                    width: parent.width
                    rowSpacing: 0
                    columns: 2

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
                }
            }

            GroupBox {
                title: qsTr("Global Shadow Settings")
                Layout.fillWidth: true

                ColumnLayout {
                    width: parent.width

                    Switch {
                        id: shadowSwitch
                        checked: true
                        text: qsTr("Shadow %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
                    }

                    Switch {
                        id: shadowLinearFilterSwitch
                        enabled: shadowSwitch.checked
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
                        enabled: shadowSwitch.checked

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
                        enabled: shadowSwitch.checked && shadowSamplingSwitch.checked
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
                        enabled: shadowSwitch.checked && shadowSamplingSwitch.checked
                        visible: shadowSamplingMethodComboBox.currentValue === 1

                        Switch {
                            id: shadowStratifiedSamplingSwitch
                            text: qsTr("Stratified")
                        }
                    }

                    GridLayout {
                        width: parent.width
                        enabled: shadowSwitch.checked && shadowSamplingSwitch.checked
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

        Column {
            id: spotlightColumn
            spacing: 0
            anchors.horizontalCenter: parent.horizontalCenter

            Button {
                id: spotlightToggler
                checkable: true
                checked: false
                text: qsTr("Spotlight")
                width: spotlightItem.width
            }

            Item {
                id: spotlightItem
                visible: spotlightToggler.checked
                implicitWidth: childrenRect.width
                implicitHeight: childrenRect.height

                ColumnLayout {
                    anchors.top: parent.top

                    Switch {
                        id: spotlightSwitch
                        Layout.fillWidth: true
                        text: qsTr("Power %1").arg(checked? qsTr("On"): qsTr("Off"))
                    }

                    GroupBox {
                        title: qsTr("Attenuate Coefficients")
                        Layout.fillWidth: true

                        GridLayout {

                            Label {
                                Layout.row: 0
                                Layout.column: 0
                                Layout.alignment: Qt.AlignLeading
                                text: qsTr("Constant")
                            }

                            Slider {
                                id: spotlightConstantSlider
                                Layout.row: 0
                                Layout.column: 1
                                Layout.alignment: Qt.AlignCenter
                                Layout.fillWidth: true
                                orientation: Qt.Horizontal
                                from: 1
                                to: 10
                                value: spotlightConstantSpinBox.value
                                stepSize: 1e-2
                                onValueChanged: { spotlightConstantSpinBox.value = value }
                            }

                            RealSpinBox {
                                id: spotlightConstantSpinBox
                                Layout.row: 0
                                Layout.column: 2
                                Layout.alignment: Qt.AlignCenter
                                Layout.fillWidth: true
                                from: spotlightConstantSlider.from
                                to: spotlightConstantSlider.to
                                stepSize: spotlightConstantSlider.stepSize
                                value: 5
                            }

                            Label {
                                Layout.row: 1
                                Layout.column: 0
                                Layout.alignment: Qt.AlignLeading
                                text: qsTr("Linear")
                            }

                            Slider {
                                id: spotlightLinearSlider
                                Layout.row: 1
                                Layout.column: 1
                                Layout.alignment: Qt.AlignCenter
                                Layout.fillWidth: true
                                orientation: Qt.Horizontal
                                from: 0
                                to: 10
                                value: spotlightLinearSpinBox.value
                                stepSize: 1e-2
                                onValueChanged: { spotlightLinearSpinBox.value = value }
                            }

                            RealSpinBox {
                                id: spotlightLinearSpinBox
                                Layout.row: 1
                                Layout.column: 2
                                Layout.alignment: Qt.AlignCenter
                                Layout.fillWidth: true
                                from: spotlightLinearSlider.from
                                to: spotlightLinearSlider.to
                                stepSize: spotlightLinearSlider.stepSize
                                value: 0
                            }

                            Label {
                                Layout.row: 2
                                Layout.column: 0
                                Layout.alignment: Qt.AlignLeading
                                text: qsTr("Quadratic")
                            }

                            Slider {
                                id: spotlightQuadraticSlider
                                Layout.row: 2
                                Layout.column: 1
                                Layout.alignment: Qt.AlignCenter
                                Layout.fillWidth: true
                                orientation: Qt.Horizontal
                                from: 0
                                to: 10
                                value: spotlightQuadraticSpinBox.value
                                stepSize: 1e-2
                                onValueChanged: { spotlightQuadraticSpinBox.value = value }
                            }

                            RealSpinBox {
                                id: spotlightQuadraticSpinBox
                                Layout.row: 2
                                Layout.column: 2
                                Layout.alignment: Qt.AlignCenter
                                Layout.fillWidth: true
                                from: spotlightQuadraticSlider.from
                                to: spotlightQuadraticSlider.to
                                stepSize: spotlightQuadraticSlider.stepSize
                                value: 0
                            }


                        }
                    }

                    CutoffGroupBox {
                        id: spotlightCutoffGroupBox
                    }

                    RadiantFluxGroupBox {
                        id: spotlightRadiantFluxGroupBox
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                    }
                }
            }
        }
    }
}
