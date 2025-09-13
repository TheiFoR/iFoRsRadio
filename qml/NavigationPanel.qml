import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Enums

ColumnLayout{
    id: root

    spacing: 10

    UButton{
        id: radioButton

        Layout.fillWidth: true
        Layout.fillHeight: true

        Layout.maximumHeight: 30
        Layout.minimumHeight: 30

        text: qsTr("Radio stations")
        font.pixelSize: 16

        icon{
            source: "qrc:/assets/icons/airwave.svg"
            width: 20
            height: 20
        }

        normalColor: core.currentPage === Pages.RadioStations ? UStyle.neutral700 : UStyle.neutral800

        onClicked:{
            core.currentPage = Pages.RadioStations
        }
    }
    UButton{
        id: roomsButton

        Layout.fillWidth: true
        Layout.fillHeight: true

        Layout.maximumHeight: 30
        Layout.minimumHeight: 30

        enabled: false

        text: qsTr("Rooms")
        font.pixelSize: 16

        icon{
            source: "qrc:/assets/icons/planet.svg"
            width: 20
            height: 20
        }

        normalColor: core.currentPage === Pages.Rooms ? UStyle.neutral700 : UStyle.neutral800

        onClicked:{
            core.currentPage = Pages.Rooms
        }
    }
    UButton{
        id: settingsButton

        Layout.fillWidth: true
        Layout.fillHeight: true

        Layout.maximumHeight: 30
        Layout.minimumHeight: 30

        enabled: false

        text: qsTr("Settings")
        font.pixelSize: 16

        icon{
            source: "qrc:/assets/icons/settings.svg"
            width: 20
            height: 20
        }

        normalColor: core.currentPage === Pages.Settings ? UStyle.neutral700 : UStyle.neutral800

        onClicked:{
            core.currentPage = Pages.Settings
        }
    }
    UButton{
        id: aboutButton

        Layout.fillWidth: true
        Layout.fillHeight: true

        Layout.maximumHeight: 30
        Layout.minimumHeight: 30

        enabled: false

        text: qsTr("About")
        font.pixelSize: 16

        icon{
            source: "qrc:/assets/icons/info.svg"
            width: 20
            height: 20
        }

        normalColor: core.currentPage === Pages.About ? UStyle.neutral700 : UStyle.neutral800

        onClicked:{
            core.currentPage = Pages.About
        }
    }
}
