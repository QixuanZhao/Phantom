import QtQuick
import QtQuick.Controls

Control {
    id: item

    property int decimals: 2
    property real value: from
    property real from: 0.0
    property real to: 10.0
    property real stepSize: 1e-2
    property bool editable: true
    property bool wrap: false

    onValueChanged: { spinbox.value = value * spinbox.scale }

    contentItem: SpinBox {
        id: spinbox

        property real scale: Math.pow(10, item.decimals)

        from: item.from * scale
        value: from
        to: item.to * scale
        stepSize: item.stepSize * scale
        editable: item.editable
        wrap: item.wrap
        inputMethodHints: Qt.ImhFormattedNumbersOnly

        validator: DoubleValidator {
            decimals: item.decimals
            bottom: Math.min(item.from, item.to)
            top:  Math.max(item.from, item.to)
        }

        textFromValue: v => Number(v / scale).toFixed(item.decimals)
        valueFromText: t => Number.parseFloat(t) * scale
//        textFromValue: v => Number(v).toString()
//        valueFromText: t => Number.parseFloat(t)
        onValueChanged: { item.value = value / scale }
    }
}
