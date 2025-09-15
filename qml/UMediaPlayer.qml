import QtQuick
import QtQuick.Layouts

import UStyle
import Enums

Rectangle{
    id: mediaplayerContainer

    implicitHeight: 40
    implicitWidth: 400

    radius: 15

    color: UStyle.neutral800

    RowLayout{
        anchors{
            fill: parent
            margins: 10
        }

        spacing: 0

        Text{
            text: core.mediaplayer.currentTitle
            font: UStyle.primaryFontRegular16
            color: UStyle.neutral100
        }

        Item{
            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Row{
            id: controlsContainer

            Layout.fillWidth: false
            Layout.fillHeight: true

            spacing: 10

            UIconButton{
                anchors{
                    verticalCenter: parent.verticalCenter
                }
                height: 26
                width: height
                iconColor: UStyle.neutral100
                iconSource:{
                    switch(core.mediaplayer.currentState){
                        case PlayStates.Ready:
                        case PlayStates.Paused:
                            return "qrc:/assets/icons/play.svg"
                        case PlayStates.Playing:
                            return "qrc:/assets/icons/pause.svg"
                    }
                }

                onClicked:{
                    switch(core.mediaplayer.currentState){
                        case PlayStates.Ready:
                        case PlayStates.Paused:
                            core.mediaplayer.play()
                            break
                        case PlayStates.Playing:
                            core.mediaplayer.pause()
                            break
                    }
                }
            }
            UIconButton{
                anchors{
                    verticalCenter: parent.verticalCenter
                }
                height: 26
                width: height
                iconColor: UStyle.neutral100
                iconSource: "qrc:/assets/icons/sound.svg"
            }
        }
    }
}
