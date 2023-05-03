import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

GroupBox {
    title: qsTr("Cut-Offs")
    Layout.fillWidth: true

    property real innerCutoff: spotlightCutOffSlider.first.value
    property real outerCutoff: spotlightCutOffSlider.second.value

    RowLayout {

        Label {
            Layout.alignment: Qt.AlignTrailing
            text: qsTr("Inner Cut-Off")
        }

        RangeSlider {
            Layout.fillWidth: true
            id: spotlightCutOffSlider
            from: 0
            to: Math.PI / 6
            first.value: Math.PI / 12
            second.value: Math.PI / 10
        }

        Label {
            Layout.alignment: Qt.AlignLeading
            text: qsTr("Outer Cut-Off")
        }
    }

}
