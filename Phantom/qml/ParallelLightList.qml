import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

ListView {
    id: listView
    anchors.fill: parent
    focus: true
    clip: true
    model: ParallelLightModel
    headerPositioning: ListView.OverlayHeader

//    header: Pane {
//        padding: 0
//        width: parent.width
//        z: 2
//        RowLayout{
//            width: parent.width
//            RoundButton {
//                flat: true
//                enabled: false
//                icon.source: "qrc:/static/res/icons/rainy_heavy_FILL0_wght400_GRAD0_opsz48.svg"
//                NumberAnimation on rotation {
//                    from: 0
//                    to: 360
//                    duration: 8000
//                    loops: Animation.Infinite
//                }
//            }
//            Label {
//                enabled: model.count > 0
//                text: qsTr("Parallel Light")
//                Layout.fillWidth: true
//            }

//            RoundButton {
//                flat: true
//                enabled: model.count > 0
//                icon.source: "qrc:/static/res/icons/clear_all_FILL0_wght400_GRAD0_opsz48.svg"
//                display: AbstractButton.IconOnly
//                Material.foreground: Material.Red
//                ToolTip.visible: enabled && hovered
//                ToolTip.text: qsTr("Clear All")
//                onClicked: model.removeRows(0, model.count)
//            }
//        }
//    }

    delegate: RowLayout {
        id: listItem
        width: parent.width
        spacing: 0

        readonly property bool isCurrentItem: ListView.isCurrentItem

        Dialog {
            id: dialog
            title: qsTr("%1 Properties").arg(name)
            anchors.centerIn: Overlay.overlay
            modal: false
            Material.background: Qt.rgba(0, 0, 0, 0)

            ColumnLayout {
                DirectionGroupBox {
                    id: directionGroupBox
                    onAzimuthChanged: direction.x = azimuth
                    onAltitudeChanged: direction.y = altitude

                    Component.onCompleted: {
                        direction = Qt.vector2d(azimuth, altitude);
                    }
                }

                RadiantFluxGroupBox {
                    id: radiantFluxGroupBox
                    onFluxChanged: {
                        specular = flux
                        diffuse = flux
                        radiant_flux = flux
                    }

                    Component.onCompleted: {
                        specular = diffuse = radiant_flux = flux
                    }
                }
            }
        }

        RoundButton {
            flat: true
            enabled: false
            icon.source: "qrc:/static/res/icons/rainy_heavy_FILL0_wght400_GRAD0_opsz48.svg"
            NumberAnimation on rotation {
                from: 0
                to: 360
                duration: 8000
                loops: Animation.Infinite
            }
        }

        Label {
            id: label
            text: name
            visible: true
            Layout.fillWidth: true
            Layout.leftMargin: 12

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    listView.currentIndex = index
                    listItem.isCurrentItem = true
                }
                onDoubleClicked: event => {
                    if (event.button === Qt.LeftButton) {
                        textField.visible = true
                        textField.forceActiveFocus()
                        textField.selectAll()
                        parent.visible = false
                    }
                }
            }
        }

        TextField {
            id: textField
            text: name
            visible: false
            focus: true
            Layout.fillWidth: true

            onActiveFocusChanged: {
                if (!activeFocus) {
                    label.visible = true
                    visible = false
                    name = text
                }
            }

            onEditingFinished: {
                label.visible = true
                visible = false
                name = text
            }
        }

        RoundButton {
            flat: true
            icon.source: "qrc:/static/res/icons/settings_FILL0_wght400_GRAD0_opsz48.svg"
            display: AbstractButton.IconOnly
            onClicked: { dialog.open() }
        }
        RoundButton {
            flat: true
            icon.source: "qrc:/static/res/icons/delete_FILL0_wght400_GRAD0_opsz48.svg"
            display: AbstractButton.IconOnly
            Material.foreground: Material.Red
            onClicked: { listView.model.removeRows(index, 1) }
        }
    }
    highlight: Pane { Material.background: Material.color(Material.Grey, Material.ShadeA700) }
    ScrollIndicator.vertical: ScrollIndicator {}
}
