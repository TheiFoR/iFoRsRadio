import QtQuick
import QtQuick.Controls.impl

import UStyle

Row {
    id: root

    property alias text: logoText.text
    property alias fontSize: logoText.font.pixelSize

    spacing: 5

    ColorImage{
        id: logoIcon

        width: 30
        height: width

        source: "qrc:/assets/icons/app/icon.svg"

        sourceSize{
            width: logoIcon.width
            height: logoIcon.height
        }
    }

    Text{
        id: logoText

        anchors{
            verticalCenter: parent.verticalCenter
        }

        text: "IFORSRADIO"

        font: UStyle.primaryFontBold24

        color: UStyle.neutral100
    }
}
