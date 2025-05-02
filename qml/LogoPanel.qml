import QtQuick

Row {
    id: root

    spacing: 5

    Rectangle{
        id: logoRectangle

        height: 30
        width: 30

        radius: 10

        color: "#4465FC"
    }

    Text{
        id: logoText

        text: "IFORSRADIO"

        font.pixelSize: 24
        font.bold: true
    }
}
