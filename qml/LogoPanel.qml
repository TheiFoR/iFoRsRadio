import QtQuick

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

        color: "#4465FC"
    }

    Text{
        id: logoText

        anchors{
            verticalCenter: parent.verticalCenter
        }

        text: "IFORSRADIO"

        font.pixelSize: 24
        font.bold: true
    }
}
