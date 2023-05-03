import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

TreeView {
    id: treeView
    anchors.fill: parent
    focus: true
    clip: true
    model: AnalyticalLightTreeModel

    function typeIcon (type: string) {
        if (type === 'Parallel Light') return "qrc:/static/res/icons/rainy_heavy_FILL0_wght400_GRAD0_opsz48.svg"
        if (type === 'Point Light') return "qrc:/static/res/icons/lightbulb_FILL0_wght400_GRAD0_opsz48.svg"
        if (type === 'Spotlight') return "qrc:/static/res/icons/highlight_FILL0_wght400_GRAD0_opsz48.svg"
        else return "qrc:/static/res/icons/chevron_right_FILL0_wght400_GRAD0_opsz48.svg"
    }

    function typeIndex (type: string) {
        if (type === '\nD' || type === 'Parallel Light') return 0
        if (type === '\nP' || type === 'Point Light') return 1
        if (type === '\nS' || type === 'Spotlight') return 2
        else return -1
    }

    function typeCount (type: string) {
        if (type === '\nD') return ParallelLightModel.count;
        if (type === '\nP') return PointLightModel.count
        if (type === '\nS') return SpotlightModel.count
        else return 0
    }

    function isSpotlight (type: string) { return type === 'Spotlight' }
    function isPointLight (type: string) { return type === 'Point Light' }
    function isParallelLight (type: string) { return type === 'Parallel Light' }

    Dialog {
        id: dialog

        property string type
        property string name
        property int index_row

        title: qsTr("%1 Properties").arg(name)
        anchors.centerIn: Overlay.overlay
        modal: false
        Material.background: Qt.rgba(0, 0, 0, 0)

        ColumnLayout {
            DirectionGroupBox {
                id: directionGroupBox
                visible: isSpotlight(dialog.type) || isParallelLight(dialog.type)
            }

            Label {
                text: "%1, %2".arg(spotlightCutoffGroupBox.innerCutoff).arg(spotlightCutoffGroupBox.outerCutoff)
            }

            CutoffGroupBox {
                id: spotlightCutoffGroupBox
                visible: isSpotlight(dialog.type)
            }

            RadiantFluxGroupBox {
                id: radiantFluxGroupBox
            }
        }
    }

    delegate: TreeViewDelegate {
        id: treeNode
        implicitWidth: treeView.width
        leftPadding: indicator.width
        indentation: 0

        property bool isCurrent: dialog.index_row === index_row && dialog.type === type

        onIsCurrentChanged: {
            if (isSpotlight(type)) {
                if (isCurrent) {
//                    directionGroupBox.azimuth = direction.x
//                    directionGroupBox.altitude = direction.y
//                    spotlightCutoffGroupBox.innerCutoff = cutoff.x
//                    spotlightCutoffGroupBox.outerCutoff = cutoff.y
                    direction = Qt.binding(() => Qt.vector2d(directionGroupBox.azimuth, directionGroupBox.altitude))
                    cutoff = Qt.binding(() => Qt.vector2d(spotlightCutoffGroupBox.innerCutoff, spotlightCutoffGroupBox.outerCutoff))
                } else {
                    direction = direction
                    cutoff = cutoff
                }
            } else if (isParallelLight(type)) {
                if (isCurrent) {
//                    directionGroupBox.azimuth = direction.x
//                    directionGroupBox.altitude = direction.y
                    direction = Qt.binding(() => Qt.vector2d(directionGroupBox.azimuth, directionGroupBox.altitude))
                } else {
                    direction = direction
                }
            }

            if (isCurrent) {
//                radiantFluxGroupBox.colour = colour
//                radiantFluxGroupBox.power = power
                colour = Qt.binding(() => radiantFluxGroupBox.colour)
                power = Qt.binding(() => radiantFluxGroupBox.power)
            } else {
                colour = colour
                power = power
            }
        }

        indicator: RoundButton {
            flat: true
            icon.source: treeView.typeIcon(type)
            rotation: treeNode.expanded ? 90 : 0
            onClicked: {
                if (treeView.isExpanded(row))
                    treeView.collapse(row)
                else
                    treeView.expand(row)
            }
        }

        background: Pane {
            Material.background: depth === 0 ? Material.color(Material.Grey, Material.ShadeA700) : Material.color(Material.Grey, Material.Shade900)
        }

        contentItem: RowLayout {

            width: treeView.width

            Label {
                visible: depth === 0
                enabled: typeCount(type) > 0
                text: name
                Layout.fillWidth: true
            }

            RoundButton {
                flat: true
                enabled: false
                visible: depth !== 0
                icon.source: typeIcon(type)
                NumberAnimation on rotation {
                    from: 0
                    to: 360
                    duration: 6000 * Math.random() + 6000
                    loops: Animation.Infinite
                }
            }

            Label {
                id: label
                enabled: true
                visible: depth !== 0 && enabled
                text: name
                Layout.fillWidth: true

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        parent.enabled = false
                        textField.forceActiveFocus()
                        textField.selectAll()
                    }
                }
            }
            TextField {
                id: textField
                visible: !label.enabled
                text: name
                onEditingFinished: {
                    name = text
                    label.enabled = true
                }
            }


            RoundButton {
                flat: true
                visible: depth === 0
                icon.source: "qrc:/static/res/icons/add_FILL0_wght400_GRAD0_opsz48.svg"
                onClicked: {
                    console.log(type)
                    console.log(typeIndex(type))
                    treeView.model.insertRows(typeCount(type), 1, treeView.model.index(typeIndex(type), 0))
                }
            }
            RoundButton {
                flat: true
                visible: depth === 0
                enabled: typeCount(type) > 0
                icon.source: "qrc:/static/res/icons/clear_all_FILL0_wght400_GRAD0_opsz48.svg"
                Material.foreground: Material.Red
                onClicked: {
                    treeView.model.removeRows(0, typeCount(type), treeView.model.index(typeIndex(type), 0))
                }
            }

            RoundButton {
                visible: isSpotlight(type) || isPointLight(type)
                checkable: true
                highlighted: checked
                checked: isSpotlight(type) && controller.lighting.pickedSpotlightIndex === index_row || isPointLight(type) && controller.lighting.pickedPointLightIndex === index_row
                icon.source: "qrc:/static/res/icons/drag_pan_FILL0_wght400_GRAD0_opsz48.svg"
                onClicked: {
                    if (isSpotlight(type)) {
                        if (controller.lighting.pickedSpotlightIndex === index_row) controller.lighting.pickedSpotlightIndex = -1
                        else {
                            controller.lighting.pickedPointLightIndex = -1
                            controller.lighting.pickedSpotlightIndex = index_row
                        }
                    } else if (isPointLight(type)) {
                        if (controller.lighting.pickedPointLightIndex === index_row) controller.lighting.pickedPointLightIndex = -1
                        else {
                            controller.lighting.pickedSpotlightIndex = -1
                            controller.lighting.pickedPointLightIndex = index_row
                        }
                    }
                }
            }

            RoundButton {
                flat: true
                visible: depth !== 0
                icon.source: "qrc:/static/res/icons/settings_FILL0_wght400_GRAD0_opsz48.svg"
                onClicked: {
                    dialog.name = name
                    dialog.index_row = -1

                    if (isSpotlight(type) || isParallelLight(type)) {
                        console.log(direction.x, direction.y)
                        directionGroupBox.azimuth = direction.x
                        directionGroupBox.altitude = direction.y
                        if (isSpotlight(type)) {
                            console.log("cutoff", cutoff.x, cutoff.y)
                            spotlightCutoffGroupBox.innerCutoff = cutoff.x
                            spotlightCutoffGroupBox.outerCutoff = cutoff.y
                        }
                    }
                    radiantFluxGroupBox.colour = colour
                    radiantFluxGroupBox.power = power

                    dialog.type = type
                    dialog.index_row = index_row

                    dialog.open()
                }
            }

            RoundButton {
                flat: true
                visible: depth !== 0
                icon.source: "qrc:/static/res/icons/delete_FILL0_wght400_GRAD0_opsz48.svg"
                Material.foreground: Material.Red
                onClicked: {
                    treeView.model.removeRows(index_row, 1, treeView.model.index(typeIndex(type), 0))
                }
            }
        }
    }

    ScrollIndicator.vertical: ScrollIndicator {}
}
