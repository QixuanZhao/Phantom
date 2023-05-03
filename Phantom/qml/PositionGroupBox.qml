import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

GroupBox {
    title: qsTr("Position")
    Layout.fillWidth: true

    property vector3d value: Qt.vector3d(pointLightXSpinBox.value, pointLightYSpinBox.value, pointLightZSpinBox.value)

    RowLayout {
        width: parent.width
        GridLayout {
            Layout.fillWidth: true
            Label {
                Layout.row: 0
                Layout.column: 0
                text: "X"
            }
            RealSpinBox {
                id: pointLightXSpinBox
                Layout.row: 0
                Layout.column: 1
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true
                value: 1e-6
                from: -2e6
                to: 2e6
                decimals: 3
                stepSize: 1e-3
            }
            Label {
                Layout.row: 1
                Layout.column: 0
                text: "Y"
            }
            RealSpinBox {
                id: pointLightYSpinBox
                Layout.row: 1
                Layout.column: 1
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true
                from: -2e6
                to: 2e6
                value: 1e-6
                decimals: 3
                stepSize: 1e-3
            }
            Label {
                Layout.row: 2
                Layout.column: 0
                text: "Z"
            }
            RealSpinBox {
                id: pointLightZSpinBox
                Layout.row: 2
                Layout.column: 1
                Layout.alignment: Qt.AlignCenter
                Layout.fillWidth: true
                from: -2e6
                to: 2e6
                value: -5
                decimals: 3
                stepSize: 1e-3
            }
        }
    }
}
