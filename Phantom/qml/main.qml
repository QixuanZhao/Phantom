import QtCore
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
        Menu {
            title: qsTr("&Interface")
            Action {
                icon.source: "qrc:/static/res/icons/translate_FILL0_wght400_GRAD0_opsz48.svg"
                text: qsTr("&Language")
                onTriggered: languageDialog.open()
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

    Dialog {
        id: languageDialog

        title: qsTr("Language")
        anchors.centerIn: Overlay.overlay

        RowLayout {
            Label { text: qsTr("Current Language") }
            ComboBox {
                id: localeComboBox
                textRole: "text"
                valueRole: "value"
                model: languageModel
                currentIndex: 0
                onCurrentValueChanged: {
                    controller.retranslate(Qt.locale(currentValue));
                }
            }
        }
    }

    ListModel {
        id: languageModel
        ListElement { text: qsTr("Simplified Chinese"); value: "zh_CN" }
        ListElement { text: qsTr("Traditional Chinese"); value: "zh_TW" }
        ListElement { text: qsTr("English"); value: "en" }
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
            Layout.minimumWidth: stack.implicitWidth
            Layout.maximumWidth: stack.implicitWidth
            Layout.fillHeight: true

            orientation: Qt.Vertical

            Pane {
                id: leftTopItem
                SplitView.fillHeight: true
                SplitView.fillWidth: true
                implicitWidth: childrenRect.width
                padding: 0

                ColumnLayout {
                    id: leftColumn
                    anchors.fill: parent
                    spacing: 0

                    TabBar {
                        id: tabBar
                        Layout.fillWidth: true
                        Layout.minimumHeight: childrenRect.height

                        TabButton { text: qsTr("Display") }
                        TabButton { text: qsTr("Camera") }
                        TabButton { text: qsTr("Lighting") }
                        TabButton { text: qsTr("Graphics") }
                    }

                    StackLayout {
                        id: stack
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.topMargin: 6
                        currentIndex: tabBar.currentIndex

                        property int maxContentWidth: Math.max(displayScrollView.width, cameraScrollView.width, lightingScrollView.width, graphicsScrollView.width)

                        Component.onCompleted: {
                            let panels = [displayScrollView, cameraScrollView, lightingScrollView, graphicsScrollView]
                            for (let i in panels) {
                                console.log("loop:", i, panels[i].width, maxContentWidth, panels[i].contentWidth, panels[i].availableWidth)
                                if (panels[i].contentWidth !== maxContentWidth) {
                                    panels[i].contentWidth = Qt.binding(() => panels[i].availableWidth)
                                }
                            }
                        }

                        ScrollView {
                            id: displayScrollView
                            ScrollBar.vertical.interactive: false
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            DisplayPanel {
                                id: displayPanel
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }

                        ScrollView {
                            id: cameraScrollView
                            ScrollBar.vertical.interactive: false
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            CameraPanel {
                                id: cameraPanel
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }

                        ScrollView {
                            id: lightingScrollView
                            ScrollBar.vertical.interactive: false
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            LightingPanel {
                                id: lightingPanel
                                anchors.horizontalCenter: parent.horizontalCenter
                            }
                        }

                        ScrollView {
                            id: graphicsScrollView
                            ScrollBar.vertical.interactive: false
                            Layout.alignment: Qt.AlignHCenter
                            Layout.fillWidth: true
                            Layout.fillHeight: true

                            GraphicsPanel {
                                id: graphicsPanel
                                anchors.horizontalCenter: parent.horizontalCenter
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
                focus: true
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton
                cursorShape: Qt.OpenHandCursor
                hoverEnabled: true

                property bool observerMode: controller.cameraType === Controller.CameraType.Observer
                property bool catchMouse: false

                function setCenter () {
                    controller.setMouseCenter(mapToGlobal(width / 2, height / 2))
                }

                onWidthChanged: setCenter()
                onHeightChanged: setCenter()
                onPositionChanged: setCenter()

                onObserverModeChanged: {
                    if (observerMode) {
                        cursorShape = Qt.OpenHandCursor
                        catchMouse = false
                    } else {
                        cursorShape = Qt.ArrowCursor
                    }
                }

                Keys.onPressed: event => {
                    if (!event.isAutoRepeat)
                        controller.pressKey(event.key);
                    if (event.modifiers & Qt.ControlModifier) {
                        if (!observerMode) {
                            catchMouse = !catchMouse
                            cursorShape = catchMouse ? Qt.BlankCursor : Qt.ArrowCursor

                            if (catchMouse) {
                                controller.grabCursor()
                                controller.camera.setMouse(mapToGlobal(Qt.point(mouseX, mouseY)))
                            } else controller.releaseCursor()

                            controller.camera.viewLock = !catchMouse
                            event.accepted = true
                        }
                    }
                }

                Keys.onReleased: event => {
                    if (!event.isAutoRepeat) controller.releaseKey(event.key);
                }

                onPressed: (mouse) => {
                    controller.pressButton(mouse.button)
                    forceActiveFocus()
                }
                onReleased: (mouse) => {
                    controller.releaseButton(mouse.button)
                    forceActiveFocus()
                }
                onWheel: (wheel) => {
                    controller.camera.wheel(wheel.angleDelta)
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
                    text: qsTr("FPS: %1").arg(renderer.fps)
                }

                Label {
                    text: qsTr("Mouse %1 (%2)").arg(rendererMouse.catchMouse ? qsTr("Trapped") : qsTr("Free")).arg(
                        rendererMouse.observerMode ? qsTr("Change to FPS camera to enable") : (
                            rendererMouse.activeFocus ? qsTr("press CTRL to toggle") : qsTr("unfocused")
                        )
                    )

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
