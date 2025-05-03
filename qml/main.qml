import QtQuick
import QtQuick.Layouts

import Enums

Window {
    width: 880
    height: 550
    visible: true
    title: qsTr("iFoRsRadio")


    RowLayout{
        id: rootContainer

        anchors{
            fill: parent
            margins: 10
        }

        spacing: 10

        ColumnLayout{
            id: leftSideContainer

            Layout.maximumWidth: 200
            Layout.minimumWidth: 200

            Layout.fillHeight: true

            spacing: 20

            LogoPanel{
                id: logoPanel

                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.maximumHeight: 30
            }

            NavigationPanel{
                id: navigationPanel

                Layout.fillWidth: true
                Layout.fillHeight: false
            }

            Item{
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            Text{
                id: serverConnectionStatus

                Layout.fillWidth: true
                Layout.fillHeight: false

                text: {
                    let text = qsTr("Server status") + ": "
                    if(core.server.connectionStatus === ConnectionStatuses.NoConnection){
                        return text + qsTr("No connection")
                    }
                    else if(core.server.connectionStatus === ConnectionStatuses.Connecting){
                        return text + qsTr("Connecting")
                    }
                    else if(core.server.connectionStatus === ConnectionStatuses.Connected){
                        return text + qsTr("Connected")
                    }

                    return text + qsTr("Unknown")
                }
            }
        }

        StackLayout{
            id: pagesLayout

            currentIndex: core.currentPage

            RadioStationPage{}
            RoomsPage{}
            SettingsPage{}
        }
    }
}
