import QtQuick
import QtQuick.Layouts

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

        Column{
            id: leftSideContainer

            Layout.maximumWidth: 200
            Layout.minimumWidth: 200

            Layout.fillHeight: true

            spacing: 20

            LogoPanel{
                id: logoPanel
            }

            NavigationPanel{
                id: navigationPanel
            }
        }

        StackLayout{
            id: pagesLayout

            currentIndex: core.currentPage

            HomePage{}
            RoomsPage{}
            SettingsPage{}
        }
    }
}
