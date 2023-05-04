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
    }

    RowLayout {
        id: graphicsPanel
        anchors.horizontalCenter: parent.horizontalCenter
        spacing: 6
    }

}
