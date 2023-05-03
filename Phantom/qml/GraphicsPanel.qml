import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

Item {
    id: graphicsScrollItem
    implicitWidth: childrenRect.width + 20
    implicitHeight: childrenRect.height + 30

    Component.onCompleted: {
        controller.graphics.deferredRendering = Qt.binding(() => deferredRenderingSwitch.checked)
        controller.graphics.pbrEnabled = Qt.binding(() => pbrSwitch.checked)
    }

    RowLayout {
        id: graphicsPanel
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 6

        Switch {
            id: deferredRenderingSwitch
            checked: true
            text: qsTr("Deferred Rendering %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
        }

        Switch {
            id: pbrSwitch
            checked: true
            text: qsTr("PBR %1").arg(checked ? qsTr("Enabled") : qsTr("Disabled"))
        }
    }

}
