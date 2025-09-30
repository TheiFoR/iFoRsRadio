import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic
import QtQuick.Controls.impl

import UStyle

Button {
    id: root

    property color normalColor: "transparent"
    property color hoverColor: normalColor
    property color pressedColor: normalColor
    property color disabledColor: normalColor

    property color textNormalColor: UStyle.neutral400
    property color textHoverColor: UStyle.neutral100
    property color textPressedColor: UStyle.neutral100
    property color textDisabledColor: UStyle.neutral700

    property bool selected: false
    property int radius: Math.min(height, width) / 2

    font: UStyle.primaryFontRegular16
    spacing: 10
    padding: 10

    background: Rectangle {
        id: backgroundRect
        anchors.fill: parent
        radius: root.radius
        color: root.normalColor

        MouseArea {
            anchors.fill: parent
            hoverEnabled: true
            acceptedButtons: Qt.NoButton
            cursorShape: (root.enabled && !root.selected) ? Qt.PointingHandCursor : Qt.ArrowCursor
        }
    }

    contentItem: Row {
        id: contentRow
        spacing: root.spacing

        ColorImage {
            id: iconImage
            anchors.verticalCenter: parent.verticalCenter
            source: root.icon.source
            width: Math.min(root.icon.height, root.icon.width)
            height: Math.min(root.icon.height, root.icon.width)
            sourceSize.width: width
            sourceSize.height: height
            fillMode: Image.PreserveAspectFit
            color: root.textNormalColor
            visible: source !== ""
        }

        Text {
            id: buttonText
            anchors.verticalCenter: parent.verticalCenter
            text: root.text
            font: root.font
            color: root.textNormalColor
        }
    }

    states: [
        State {
            name: "disabled"
            when: !root.enabled
            PropertyChanges { backgroundRect.color: root.disabledColor }
            PropertyChanges { iconImage.color: root.textDisabledColor }
            PropertyChanges { buttonText.color: root.textDisabledColor }
        },
        State {
            name: "pressed"
            when: root.pressed || root.selected
            PropertyChanges { backgroundRect.color: root.pressedColor }
            PropertyChanges { iconImage.color: root.textPressedColor }
            PropertyChanges { buttonText.color: root.textPressedColor }
        },
        State {
            name: "hovered"
            when: root.hovered
            PropertyChanges { backgroundRect.color: root.hoverColor }
            PropertyChanges { iconImage.color: root.textHoverColor }
            PropertyChanges { buttonText.color: root.textHoverColor }
        },
        State {
            name: "normal"
            when: root.enabled && !root.hovered && !root.pressed && !root.selected
            PropertyChanges { backgroundRect.color: root.normalColor }
            PropertyChanges { iconImage.color: root.textNormalColor }
            PropertyChanges { buttonText.color: root.textNormalColor }
        }
    ]

    transitions: [
        Transition {
            from: "*"
            to: "*"
            ColorAnimation { properties: "color"; duration: 100 }
        }
    ]
}
