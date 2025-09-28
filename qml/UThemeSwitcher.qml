import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property int padding: 2
    property int currentIndex: 0

    signal clicked()

    Rectangle {
        id: background

        anchors{
            fill: parent
        }

        color: UStyle.neutral700
        radius: root.height / 2

        Rectangle{
            id: selectedThemeIndicator

            anchors{
                top: background.top
                left: background.left
                topMargin: root.padding
                leftMargin: {
                    if(root.currentIndex === 0){
                        return root.padding
                    }
                    else{
                        return background.width - selectedThemeIndicator.width - root.padding
                    }
                }

                Behavior on leftMargin{
                    NumberAnimation{
                        duration: 100
                        easing.type: Easing.OutQuad
                    }
                }
            }

            width: root.height - root.padding * 2
            height: width
            radius: width / 2

            color: UStyle.neutral800
        }

        RowLayout{
            id: buttonsContainer

            anchors{
                centerIn: parent
            }

            spacing: 0

            UIconButton{
                id: darkThemeButton

                Layout.minimumWidth: 16
                Layout.maximumWidth: 16
                Layout.minimumHeight: 16
                Layout.maximumHeight: 16

                iconSource: "qrc:/assets/icons/moon.svg"
                iconSize: 12

                onClicked:{
                    root.clicked()
                }
            }

            Item{
                Layout.fillHeight: true
                Layout.fillWidth: true
            }

            UIconButton{
                id: lightThemeButton

                Layout.minimumWidth: 16
                Layout.maximumWidth: 16
                Layout.minimumHeight: 16
                Layout.maximumHeight: 16

                iconSource: "qrc:/assets/icons/sun.svg"
                iconSize: 12

                onClicked:{
                    root.clicked()
                }
            }
        }
    }
}
