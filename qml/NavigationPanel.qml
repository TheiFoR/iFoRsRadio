import QtQuick
import QtQuick.Controls

import Enums

Column {
    id: root

    spacing: 23

    Button{
        id: homeButton

        text: qsTr("Radio stations")
        font.pixelSize: 16

        onClicked:{
            core.currentPage = Pages.RadioStations
        }
    }
    Button{
        id: roomsButton

        text: qsTr("Rooms")
        font.pixelSize: 16

        onClicked:{
            core.currentPage = Pages.Rooms
        }
    }
    Button{
        id: settingsButton

        text: qsTr("Settings")
        font.pixelSize: 16

        onClicked:{
            core.currentPage = Pages.Settings
        }
    }
}
