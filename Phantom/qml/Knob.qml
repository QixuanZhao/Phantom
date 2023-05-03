import QtQuick
import QtQuick.Controls

Control {
    id: knob
    padding: 6

    property real angle: 0
    property real from: 0
    property real to: 1
    property real value: from + (to - from) * angle / 360

    onAngleChanged: {
        value = from + (to - from) * angle / 360
    }

    onValueChanged: {
        angle = 360 * (value - from) / (to - from)
    }

    contentItem: Rectangle {
        id: plate
        implicitWidth: 90
        implicitHeight: 90
        width: Math.min(knob.availableWidth, knob.availableHeight)
        height: Math.min(knob.availableWidth, knob.availableHeight)
        anchors.centerIn: knob
        radius: width / 2
        color: "#f4f4f4"
        border {
            width: 1
            color: "#565656"
        }
        gradient: Gradient {
            GradientStop { position: 0; color: "#f9f9f9" }
            GradientStop { position: 0.4; color: "#f4f4f4" }
            GradientStop { position: 1; color: "#e5e5e5" }
        }

        Rectangle {
            id: pointer
            width: plate.width / 6
            height: width
            radius: width / 2
            x: plate.width / 2 - width / 2
            y: plate.height / 2 - height / 2
            color: "#e2e2e2"
            border {
                width: 1
                color: "#a0a0a0"
            }
            gradient: Gradient {
                GradientStop { position: 0; color: "#e5e5e5" }
                GradientStop { position: 1; color: "#e2e2e2" }
            }
            transform: [
                Translate { y: plate.width / 3 },
                Rotation {
                    origin { x: pointer.width / 2; y: pointer.height / 2 }
                    angle: knob.angle
                }
            ]
        }

        MouseArea {
            anchors.fill: plate
            acceptedButtons: Qt.LeftButton
            preventStealing : true
            hoverEnabled: pressed
            onPositionChanged: (mouse) => {
                let x = mouse.x - plate.width / 2
                let y = mouse.y - plate.height / 2
                let cosine = Qt.vector2d(x, y).normalized().y
                let radians = Math.acos(cosine)
                if (x > 0) radians = 2 * Math.PI - radians
                knob.angle = 180 * radians / Math.PI
            }
        }
    }
}
