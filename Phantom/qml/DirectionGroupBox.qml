import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

GroupBox {
    id: groupBox
    title: qsTr("Direction")
    Layout.fillWidth: true
    Layout.fillHeight: true

    property real azimuth: azimuthSpinBox.value
    property real altitude: altitudeSlider.value

    onAzimuthChanged: azimuthSpinBox.value = azimuth
    onAltitudeChanged: altitudeSlider.value = altitude

    GridLayout {
        anchors.fill: parent

        Item {
            Layout.row: 0
            Layout.column: 0
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.minimumHeight: childrenRect.height
            Label {
                anchors.centerIn: parent
                text: qsTr("Azimuth")
            }
        }
        Knob {
            id: azimuthKnob
            Layout.row: 1
            Layout.column: 0
            Layout.alignment: Qt.AlignCenter
            Layout.fillHeight: true
            Layout.fillWidth: true
            from: azimuthSpinBox.from
            to: azimuthSpinBox.to
            value: azimuthSpinBox.value
            onValueChanged: { azimuthSpinBox.value = value }
        }
        RealSpinBox {
            id: azimuthSpinBox
            Layout.row: 2
            Layout.column: 0
            Layout.alignment: Qt.AlignCenter
            from: -180
            to: 180
            decimals: 3
            stepSize: 1e-3
            value: 0
            wrap: true
            onValueChanged: { azimuthKnob.value = groupBox.azimuth = value }
        }

        Item {
            Layout.row: 0
            Layout.column: 1
            Layout.alignment: Qt.AlignCenter
            Layout.fillWidth: true
            Layout.minimumHeight: childrenRect.height
            Label {
                anchors.centerIn: parent
                text: qsTr("Altitude")
            }
        }
        Slider {
            id: altitudeSlider
            Layout.row: 1
            Layout.column: 1
            Layout.alignment: Qt.AlignCenter
            from: -90
            to: 90
            value: altitudeSpinBox.value
            stepSize: 1e-3
            orientation: Qt.Vertical
            onValueChanged: { altitudeSpinBox.value = groupBox.altitude = value }
        }
        RealSpinBox {
            id: altitudeSpinBox
            Layout.row: 2
            Layout.column: 1
            Layout.alignment: Qt.AlignCenter
            from: -90.0
            to: 90.0
            decimals: 3
            stepSize: 1e-3
            value: 45.0
        }
    }

}
