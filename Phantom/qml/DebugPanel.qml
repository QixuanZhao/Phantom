import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

ColumnLayout {
    id: debugPanel
    anchors.right: parent.right
    anchors.top: parent.top
    anchors.margins: 20
    spacing: 0

    Component.onCompleted: {
        controller.debug.hubAlpha = Qt.binding(() => hubAlpha.value)
        controller.debug.mix = Qt.binding(() => mix.value)
    }

    Connections {
        target: controller.debug
        onLogged: str => debugTextArea.append(str)
    }

    ScrollView {
        id: view
        Layout.maximumHeight: 300
        Layout.fillWidth: true

        TextArea {
            id: debugTextArea
            readOnly: true
            textFormat: TextEdit.RichText
        }
    }

    Button {
        text: qsTr("Reload Shaders")
        onClicked: controller.reloadShaders()
        Layout.alignment: Qt.AlignHCenter
        Layout.fillWidth: true
    }

    RowLayout {
        Label { text: qsTr("Hub Opaqueness") }

        Slider {
            id: hubAlpha
            from: 0
            to: 1
            value: 0.7
            Layout.fillWidth: true

            ToolTip {
                text: qsTr("%1").arg(hubAlpha.value)
                visible: hubAlpha.enabled && (hubAlpha.hovered || hubAlpha.pressed)
                parent: hubAlpha.handle
            }
        }
    }

    RowLayout {
        Label { text: qsTr("Mix Parameter") }

        Slider {
            id: mix
            from: 0
            to: 1
            value: 0.7
            Layout.fillWidth: true

            ToolTip {
                text: qsTr("%1").arg(mix.value)
                visible: mix.enabled && (mix.hovered || mix.pressed)
                parent: mix.handle
            }
        }
    }
}
