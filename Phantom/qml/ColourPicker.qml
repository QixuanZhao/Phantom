import QtQuick
import QtQuick.Dialogs as QD
import QtQuick.Layouts

Rectangle {
    id: picker

    color: currentColour
    property color currentColour: "white"

    implicitWidth: width
    implicitHeight: height

    border.width: 3
    border.color: Qt.rgba(picker.color.r, picker.color.g, picker.color.b, 0.7)
    radius: 5

    width: label.width + 6
    height: label.height + 6

    onCurrentColourChanged: {
        dialog.selectedColor = currentColour
    }

    MouseArea {
        anchors.fill: parent
        acceptedButtons: Qt.LeftButton
        onClicked: {
            dialog.open()
        }
    }

    QD.ColorDialog {
        id: dialog
        title: qsTr("Select Colour")
        onSelectedColorChanged: {
            picker.currentColour = selectedColor
        }
//        options: QD.ColorDialog.NoButtons
    }


    ColumnLayout {
        id: label
        anchors.centerIn: parent
        spacing: 0

        Text {
            id: text
            font { family: "SF Mono" }
            Layout.fillHeight: true
            Layout.fillWidth: true
            Layout.margins: 5
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            wrapMode: Text.WordWrap
            color: picker.currentColour.hslLightness > 0.5 ? "black" : "white"
            text: picker.currentColour
        }

    }
}
