import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Controls.impl

Button{
    id: root

    // *** Public properties ***
    property alias iconSource: iconImage.source
    property int iconSize: Math.min(height, width)

    property color iconColor: UStyle.neutral400
    property color hoverColor: UStyle.neutral100
    property color pressedColor: UStyle.neutral100
    property color disabledColor: UStyle.neutral600

    property color backgroundColor: "transparent"
    property color hoverBackgroundColor: "transparent"
    property color pressedBackgroundColor: "transparent"
    property color disabledBackgroundColor: "transparent"

    property int radius: 5

    // *** Private properties ***

    width: 40
    height: 40

    padding: 0
    leftInset: 0
    rightInset: 0
    topInset: 0

    background: Rectangle{
        anchors.fill: parent

        color: {
            if(!root.enabled){
                return root.disabledBackgroundColor
            } else if(root.pressed){
                return root.pressedBackgroundColor
            } else if(root.hovered){
                return root.hoverBackgroundColor
            } else {
                return root.backgroundColor
            }
        }

        radius: root.radius

        Behavior on color{
            ColorAnimation{
                duration: 100
            }
        }
    }

    contentItem: ColorImage{
        id: iconImage

        anchors.centerIn: parent

        width: root.iconSize
        height: root.iconSize
        sourceSize{
            width: root.iconSize
            height: root.iconSize
        }
        fillMode: Image.PreserveAspectFit
        source: ""

        color: {
            if(!root.enabled){
                return root.disabledColor
            } else if(root.pressed){
                return root.pressedColor
            } else if(root.hovered){
                return root.hoverColor
            } else {
                return root.iconColor
            }
        }

        Behavior on color{
            ColorAnimation{
                duration: 100
            }
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.NoButton        // не перехватываем клики
            cursorShape: root.enabled ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }
}
