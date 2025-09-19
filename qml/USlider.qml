import QtQuick
import QtQuick.Controls.Basic

import UStyle

Slider {
    id: control
    value: 0.5

    background: Rectangle {
        x: control.orientation === Qt.Horizontal ? control.leftPadding : control.leftPadding + control.availableWidth / 2 - width / 2
        y: control.orientation === Qt.Horizontal ? control.topPadding + control.availableHeight / 2 - height / 2 : control.topPadding
        implicitWidth: control.orientation === Qt.Horizontal ? 200 : 4
        implicitHeight: control.orientation === Qt.Horizontal ? 4 : 200
        width: control.orientation === Qt.Horizontal ? control.availableWidth : implicitWidth
        height: control.orientation === Qt.Horizontal ? implicitHeight : control.availableHeight
        radius: Math.min(width, height) / 2
        color: UStyle.neutral700

        Rectangle {
            anchors{
                bottom: parent.bottom
            }

            width: control.orientation === Qt.Horizontal ? control.visualPosition * parent.width : parent.width
            height: control.orientation === Qt.Horizontal ? parent.height : (1 - control.visualPosition) * parent.height
            color: UStyle.indigoBlue
            radius: Math.min(width, height) / 2
        }
    }

    handle: Rectangle {
        x: control.orientation === Qt.Horizontal ? control.leftPadding + control.visualPosition * (control.availableWidth - width) : control.leftPadding + control.availableWidth / 2 - width / 2
        y: control.orientation === Qt.Horizontal ? control.topPadding + control.availableHeight / 2 - height / 2 : control.topPadding + control.visualPosition * (control.availableHeight - height)

        implicitWidth: 10
        implicitHeight: 10

        radius: Math.min(implicitWidth, implicitHeight) / 2
        color: UStyle.neutral100
    }
}
