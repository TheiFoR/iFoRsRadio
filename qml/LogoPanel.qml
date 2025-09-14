import QtQuick

import UStyle

Row {
    id: root

    property alias text: logoText.text
    property alias fontSize: logoText.font.pixelSize

    spacing: 5

    Rectangle{
        id: logoRectangle

        height: parent.height
        width: parent.height

        radius: 10

        color: UStyle.indigoBlue
    }

    Text{
        id: logoText

        anchors{
            verticalCenter: parent.verticalCenter
        }

        text: "IFORRADIO"

        font: UStyle.primaryFontBold24

        color: UStyle.neutral100
    }
}
