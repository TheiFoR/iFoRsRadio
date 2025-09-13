import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Controls.impl

Button {
    id: root

    property color normalColor: "transparent"
    property color hoverColor: normalColor
    property color pressedColor: normalColor
    property color disabledColor: "transparent"
    property alias backgroundColor: backgroundRect.color

    property color textNormalColor: UStyle.neutral200
    property color textHoverColor: UStyle.neutral100
    property color textPressedColor: UStyle.neutral100
    property color textDisabledColor: UStyle.neutral700

    property int radius: Math.min(height, width) / 2

    spacing: 10

    background: Rectangle {
        id: backgroundRect

        anchors.fill: parent
        radius: root.radius

        color: {
            if(!root.enabled) {
                return root.disabledColor
            } else if (root.pressed) {
                return root.pressedColor
            } else if (root.hovered) {
                return root.hoverColor
            } else {
                return root.normalColor
            }
        }

        Behavior on color{
            ColorAnimation{
                duration: 100
            }
        }
    }

    contentItem: Row{
        id: contentRow

        spacing: root.spacing

        ColorImage{
            id: iconImage

            anchors{
                verticalCenter: parent.verticalCenter
            }

            source: root.icon.source

            width: Math.min(root.icon.height, root.icon.width)
            height: Math.min(root.icon.height, root.icon.width)

            sourceSize{
                width: iconImage.width
                height: iconImage.height
            }

            fillMode: Image.PreserveAspectFit

            color: {
                if(!root.enabled) {
                    return root.textDisabledColor
                } else if (root.pressed) {
                    return root.textPressedColor
                } else if (root.hovered) {
                    return root.textHoverColor
                } else {
                    return root.textNormalColor
                }
            }

            visible: source !== ""

            Behavior on color{
                ColorAnimation{
                    duration: 100
                }
            }
        }

        Text {
            id: buttonText

            anchors{
                verticalCenter: parent.verticalCenter
            }

            text: root.text
            font: root.font
            color: {
                if(!root.enabled) {
                    return root.textDisabledColor
                } else if (root.pressed) {
                    return root.textPressedColor
                } else if (root.hovered) {
                    return root.textHoverColor
                } else {
                    return root.textNormalColor
                }
            }
        }
    }

    padding: 10
}
