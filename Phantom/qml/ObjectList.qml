import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

ListView {
    id: modelListView
    anchors.fill: parent
    focus: true
    clip: true

    ObjectDialog { id: objectPropertyDialog }

    headerPositioning: ListView.OverlayHeader
    header: Pane {
        padding: 0
        width: parent.width
        z: 2
        RowLayout{
            width: parent.width
            RoundButton {
                flat: true
                enabled: false
                icon {
                    source: "qrc:/static/res/icons/microbiology_FILL0_wght400_GRAD0_opsz48.svg"
                }
                NumberAnimation on rotation {
                    from: 0
                    to: 360
                    duration: 2500
                    loops: Animation.Infinite
                }
            }
            Label {
                enabled: controller.model.count > 0
                text: qsTr("3D Objects")
                Layout.fillWidth: true
            }
            RoundButton {
                flat: true
                enabled: controller.model.count > 0 && controller.cameraType === Controller.CameraType.Observer
                icon.source: "qrc:/static/res/icons/travel_explore_FILL0_wght400_GRAD0_opsz48.svg"
                display: AbstractButton.IconOnly
                onClicked: { controller.camera.zoom(); }
            }

            RoundButton {
                flat: true
                enabled: controller.model.count > 0
                icon.source: "qrc:/static/res/icons/clear_all_FILL0_wght400_GRAD0_opsz48.svg"
                display: AbstractButton.IconOnly
                Material.foreground: Material.Red
                ToolTip.visible: enabled && hovered
                ToolTip.text: qsTr("Clear All")
                onClicked: {
                    while (controller.model.count > 0)
                        controller.remove(0)
                }
            }
        }
    }

    delegate: RowLayout {
        id: listItem
        width: parent.width

        readonly property bool isCurrentItem: ListView.isCurrentItem

        RoundButton {
            id: visibleButton
            enabled: !pickedButton.checked
            flat: true
            checkable: true
            checked: true
            icon.source: checked ? "qrc:/static/res/icons/visibility_FILL0_wght400_GRAD0_opsz48.svg" : "qrc:/static/res/icons/visibility_off_FILL0_wght400_GRAD0_opsz48.svg"
            display: AbstractButton.IconOnly
            onCheckedChanged: {
                controller.data.setVisible(name, checked)
            }
        }
        ItemDelegate {
            text: name
            Layout.fillWidth: true
            onClicked: modelListView.currentIndex = index
            onDoubleClicked: {
                if (controller.cameraType === Controller.CameraType.Observer)
                    controller.camera.zoom(name);
            }
        }
        RoundButton {
            id: pickedButton
            enabled: visibleButton.checked
            highlighted: checked
            icon.source: "qrc:/static/res/icons/drag_pan_FILL0_wght400_GRAD0_opsz48.svg"
            display: AbstractButton.IconOnly
            checkable: true
            checked: false
            onCheckedChanged: {
                if (checked) controller.data.pick(name)
                else controller.data.release()
            }
        }
        RoundButton {
            flat: true
            enabled: visibleButton.checked
            icon.source: "qrc:/static/res/icons/settings_FILL0_wght400_GRAD0_opsz48.svg"
            display: AbstractButton.IconOnly
            onClicked: {
                objectPropertyDialog.objectName = name
                objectPropertyDialog.open()
            }
        }
        RoundButton {
            flat: true
            icon.source: "qrc:/static/res/icons/delete_FILL0_wght400_GRAD0_opsz48.svg"
            display: AbstractButton.IconOnly
            Material.foreground: Material.Red
            onClicked: { controller.remove(index) }
        }
    }
    highlight: Pane { Material.background: Material.color(Material.Grey, Material.ShadeA700) }
    ScrollIndicator.vertical: ScrollIndicator {}
}
