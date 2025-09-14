import QtQuick
import QtQuick.Window
import QtQuick.Controls.impl

import Qt5Compat.GraphicalEffects

Window {
    id: root

    // *** Public properties ***
    property color backgroundColor: UStyle.neutral900
    property int radius: 20
    property int padding: 15
    property int iconSize: 20
    property int headerHeight: 40

    // *** Private properties ***
    readonly property int __borderMouseAreaWidth: 3
    property rect __previousGeometry: Qt.rect(x, y, width, height)
    property bool __minimized: false



    width: 800
    height: 600

    visible: true

    flags: Qt.Window | Qt.FramelessWindowHint
    color: "transparent"

    onVisibilityChanged: {
        if (root.visibility === Window.Minimized) {
            root.__minimized = true
        } else if (root.visibility === Window.Windowed && root.__minimized) {
            root.__minimized = false

            root.x = root.__previousGeometry.x
            root.y = root.__previousGeometry.y
            root.width = root.__previousGeometry.width
            root.height = root.__previousGeometry.height
        }
    }

    Rectangle {
        id: backgroundRect

        anchors.fill: parent

        color: root.backgroundColor
        radius: (root.visibility === Window.Maximized && root.visible) ? 0 : root.radius
    }

    Rectangle {
        id: topPanel

        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }

        height: root.headerHeight
        color: "transparent"

        MouseArea {
            property point pressedPosition: Qt.point(0,0)

            anchors.fill: parent

            onPositionChanged:{
                if(!pressed || root.visibility !== Window.Maximized){
                    return;
                }

                const globalMousePoint = Qt.point(root.x + mouseX, root.y + mouseY)
                const normalPositon = Qt.point(mouseX / topPanel.width, mouseY / topPanel.height)

                root.showNormal()

                root.x = globalMousePoint.x - normalPositon.x * topPanel.width
                root.y = globalMousePoint.y - normalPositon.y * topPanel.height

                root.startSystemMove()
            }

            onPressed: (mouse) => {
                if (mouse.button === Qt.LeftButton) {
                    if (root.visibility === Window.Maximized) {
                        pressedPosition = Qt.point(mouse.x, mouse.y)
                    } else {
                        root.__previousGeometry = Qt.rect(root.x, root.y, root.width, root.height)
                        root.startSystemMove()
                    }
                }
            }

            onDoubleClicked: {
                if (root.visibility === Window.Maximized) {
                    root.showNormal()
                    root.x = root.__previousGeometry.x
                    root.y = root.__previousGeometry.y
                    root.width = root.__previousGeometry.width
                    root.height = root.__previousGeometry.height
                } else {
                    root.__previousGeometry = Qt.rect(root.x, root.y, root.width, root.height)
                    root.showMaximized()
                }
            }
        }

        Row{
            anchors {
                top: parent.top
                bottom: parent.bottom
                right: parent.right
                rightMargin: root.padding
                topMargin: root.padding
            }

            spacing: 5

            UIconButton{
                anchors {
                    top: parent.top
                }
                height: root.iconSize
                width: height
                hoverColor: UStyle.neutral100
                pressedColor: UStyle.neutral100
                iconSource: "qrc:/assets/icons/window/minimize.svg"

                onClicked:{
                    root.showMinimized()
                }
            }

            UIconButton{
                anchors {
                    top: parent.top
                }
                height: root.iconSize
                width: height
                hoverColor: UStyle.neutral100
                pressedColor: UStyle.neutral100
                iconSource: "qrc:/assets/icons/window/close.svg"

                onClicked:{
                    root.close()
                }
            }
        }
    }

    MouseArea {
        anchors {
            left: parent.left
            top: parent.top
            bottom: parent.bottom
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        width: root.__borderMouseAreaWidth
        cursorShape: Qt.SizeHorCursor
        onPressed: root.startSystemResize(Qt.LeftEdge)
    }

    MouseArea {
        anchors {
            right: parent.right
            top: parent.top
            bottom: parent.bottom
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        width: root.__borderMouseAreaWidth
        cursorShape: Qt.SizeHorCursor
        onPressed: root.startSystemResize(Qt.RightEdge)
    }

    MouseArea {
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        height: root.__borderMouseAreaWidth
        cursorShape: Qt.SizeVerCursor
        onPressed: root.startSystemResize(Qt.TopEdge)
    }

    MouseArea {
        anchors {
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        height: root.__borderMouseAreaWidth
        cursorShape: Qt.SizeVerCursor
        onPressed: root.startSystemResize(Qt.BottomEdge)
    }

    MouseArea {
        anchors {
            top: parent.top
            left: parent.left
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        width: root.__borderMouseAreaWidth * 2
        height: root.__borderMouseAreaWidth * 2
        cursorShape: Qt.SizeFDiagCursor
        onPressed: root.startSystemResize(Qt.TopEdge | Qt.LeftEdge)
    }

    MouseArea {
        anchors {
            top: parent.top
            right: parent.right
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        width: root.__borderMouseAreaWidth * 2
        height: root.__borderMouseAreaWidth * 2
        cursorShape: Qt.SizeBDiagCursor
        onPressed: root.startSystemResize(Qt.TopEdge | Qt.RightEdge)
    }

    MouseArea {
        anchors {
            bottom: parent.bottom
            left: parent.left
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        width: root.__borderMouseAreaWidth * 2
        height: root.__borderMouseAreaWidth * 2
        cursorShape: Qt.SizeBDiagCursor
        onPressed: root.startSystemResize(Qt.BottomEdge | Qt.LeftEdge)
    }

    MouseArea {
        anchors {
            bottom: parent.bottom
            right: parent.right
        }
        enabled: root.visibility !== Window.Maximized
        visible: enabled
        width: root.__borderMouseAreaWidth * 2
        height: root.__borderMouseAreaWidth * 2
        cursorShape: Qt.SizeFDiagCursor
        onPressed: root.startSystemResize(Qt.BottomEdge | Qt.RightEdge)
    }
}
