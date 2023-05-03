﻿import QtCore
import QtQuick
import QtQuick.Controls
import QtQuick.Dialogs as QD
import QtQuick.Layouts
import QtQml.Models
import Phantom 1.0

ApplicationWindow {
    id: window

    width: 1500
    height: 800
    title: qsTr("Nadir")
    visible: true

    menuBar: MenuBar {
        Menu {
            title: qsTr("&File")
            Action {
                text: qsTr("&Open")
                onTriggered: fileDialog.open()
            }
        }
    }

    QD.FileDialog {
        id: fileDialog
        title: qsTr("Choose Files")
        fileMode: QD.FileDialog.OpenFiles
        nameFilters: [qsTr("3D Object (*.obj)")]
        onAccepted: {
            for (let i = 0; i < selectedFiles.length; ++i) {
                controller.load(selectedFiles[i])
            }

            controller.camera.zoom();
        }
    }

    ListModel {
        id: blurAlgorithmListModel
        ListElement { text: qsTr("Gaussian"); value: 0 }
        ListElement { text: qsTr("Box"); value: 1 }
        ListElement { text: qsTr("Kawase"); value: 2 }
        ListElement { text: qsTr("Dual"); value: 3 }
    }

    GlobalController { id: controller }

    GridLayout {
        columnSpacing: 0
        anchors.fill: parent

        SplitView {
            Layout.row: 0
            Layout.column: 0
            Layout.minimumWidth: leftColumn.implicitWidth
            Layout.maximumWidth: leftColumn.implicitWidth
            Layout.fillHeight: true

            orientation: Qt.Vertical

            Pane {
                id: leftTopItem
                SplitView.fillHeight: true
                implicitWidth: childrenRect.width
                anchors.left: leftColumn.left
                anchors.right: leftColumn.right
                padding: 0

                ColumnLayout {
                    id: leftColumn
                    anchors.top: parent.top
                    anchors.bottom: parent.bottom
                    spacing: 0

                    TabBar {
                        id: tabBar
                        Layout.fillWidth: true
                        Layout.minimumHeight: childrenRect.height

                        TabButton { text: qsTr("Display") }
                        TabButton { text: qsTr("Lighting") }
                        TabButton { text: qsTr("Graphics") }
                    }

                    StackLayout {
                        id: stack
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.topMargin: 6
                        currentIndex: tabBar.currentIndex

                        Component.onCompleted: {
                            controller.debug.log('s   width: <span style="color: grey">%1</span>'.arg(stack.width))
                            controller.debug.log('dsv width: <span style="color: pink">%1</span>'.arg(displayScrollView.width))
                            controller.debug.log('dp  width: <span style="color: pink">%1</span>'.arg(displayPanel.width))
                            controller.debug.log('lsv width: <span style="color: green">%1</span>'.arg(lightingScrollView.width))
                            controller.debug.log('lp  width: <span style="color: green">%1</span>'.arg(lightingPanel.width))
                            controller.debug.log('gsv width: <span style="color: cyan">%1</span>'.arg(graphicsScrollView.width))
                            controller.debug.log('gp  width: <span style="color: cyan">%1</span>'.arg(graphicsPanel.width))
                        }

                        ScrollView {
                            id: displayScrollView
                            ScrollBar.vertical.interactive: false
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            DisplayPanel {
                                id: displayPanel
                                implicitWidth: parent.width
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }

                        ScrollView {
                            id: lightingScrollView
                            ScrollBar.vertical.interactive: false
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            LightingPanel {
                                id: lightingPanel
                            }
                        }

                        ScrollView {
                            id: graphicsScrollView
                            ScrollBar.vertical.interactive: false
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            GraphicsPanel {
                                id: graphicsPanel
                                implicitWidth: parent.width
                            }
                        }
                    }
                }
            }

            SplitView {
                id: leftBottomItem
                orientation: Qt.Vertical
                SplitView.preferredHeight: parent.height / 2

                Pane {
                    implicitHeight: modelListView.contentHeight
                    SplitView.minimumHeight: 30
                    SplitView.fillWidth: true
                    padding: 0

                    ObjectList {
                        id: modelListView
                        model: controller.model
                    }
                }

                ColumnLayout {
                    SplitView.fillHeight: true
                    SplitView.fillWidth: true
                    spacing: 0

                    Pane {
                        id: lightColumnHeader
                        padding: 0
                        Layout.fillWidth: true
                        Layout.minimumHeight: childrenRect.height
                        z: 2
                        RowLayout {
                            width: parent.width
                            RoundButton {
                                flat: true
                                enabled: false
                                icon.source: "qrc:/static/res/icons/flare_FILL0_wght400_GRAD0_opsz48.svg"
                                NumberAnimation on rotation {
                                    from: 0
                                    to: 360
                                    duration: 4000
                                    loops: Animation.Infinite
                                }
                            }
                            Label {
                                enabled: AnalyticalLightTreeModel.count > 0
                                text: qsTr("Analytical Light")
                                Layout.fillWidth: true
                            }
                        }
                    }

                    Pane {
                        Layout.fillHeight: true
                        Layout.fillWidth: true
                        implicitHeight: analyticialTreeView.contentHeight
                        padding: 0
                        AnalyticalLightTree { id: analyticialTreeView }
                    }

                }

            }

        }

        Item {
            Layout.row: 0
            Layout.column: 1
            Layout.fillWidth: true
            Layout.fillHeight: true

            Renderer {
                id: renderer
                anchors.fill: parent
                transform: Scale { // vertical flip
                    origin.x: renderer.width / 2
                    origin.y: renderer.height / 2
                    yScale: -1
                }

                onWidthChanged: { this.setViewportSize(Qt.size(width, height)) }
                onHeightChanged: { this.setViewportSize(Qt.size(width, height)) }
            }

            MouseArea {
                id: rendererMouse
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                hoverEnabled: true
                onPressed: (mouse) => {
                    controller.camera.setMouse(Qt.point(mouse.x, mouse.y))
                    if (mouse.button === Qt.LeftButton)
                        controller.camera.viewLock = false
                    else if (mouse.button === Qt.RightButton)
                        controller.camera.translationLock = false
                }
                onReleased: (mouse) => {
                    if (mouse.button === Qt.LeftButton)
                        controller.camera.viewLock = true
                    else if (mouse.button === Qt.RightButton)
                        controller.camera.translationLock = true
                }
                onWheel: (wheel) => {
                    controller.camera.wheel(wheel.angleDelta)
                }
                onPositionChanged: (mouse) => {
                    controller.camera.mouse(Qt.point(mouse.x, mouse.y))
                }
            }

            Rectangle {
                color: Qt.rgba(0, 0, 0, controller.debug.hubAlpha)
                radius: 10
                border.width: 1
                border.color: "black"
                anchors.fill: label
                anchors.margins: -10
            }

            ColumnLayout {
                id: label
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 20
                spacing: 0

                Label {
                    color: "white"
                    wrapMode: Text.WordWrap
                    text: qsTr("FPS: %1").arg(renderer.fps)
                }

                Label {
                    color: "white"
                    wrapMode: Text.WordWrap
                    text: qsTr("Cursor: (%1, %2)").arg(rendererMouse.mouseX).arg(rendererMouse.mouseY)
                }

                Label {
                    color: "white"
                    wrapMode: Text.WordWrap
                    text: qsTr("Attitude: (%1°, %2°, %3°)").arg(controller.camera.pitch).arg(controller.camera.yaw).arg(controller.camera.roll)
                }

                Label {
                    color: "white"
                    wrapMode: Text.WordWrap
                    text: qsTr("Distance: %1").arg(controller.camera.distance)
                }

                Label {
                    color: "white"
                    wrapMode: Text.WordWrap
                    text: qsTr("Target: (%1, %2, %3)").arg(controller.camera.target.x).arg(controller.camera.target.y).arg(controller.camera.target.z)
                }
            }

            Rectangle {
                color: Qt.rgba(0, 0, 0, controller.debug.hubAlpha)
                radius: 10
                border.width: 1
                border.color: "black"
                anchors.fill: debugPanel
                anchors.margins: -10
            }

            DebugPanel { id: debugPanel }
        }

    }
}