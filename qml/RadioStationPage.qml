import QtQuick
import QtQuick.Layouts
import Qt5Compat.GraphicalEffects
import QtQuick.Effects

Item {
    id: radioStationsPageRoot

    GridView{
        id: radioStationsGrid

        readonly property int horizontalSpacing: 20
        readonly property int verticalSpacing: 20

        property int radioStationWidth: 110
        property int radioStationHeight: 110

        property real currentHorizontalSpacing:{
            const gridWidth = radioStationsGrid.width
            const ceilItemCount = parseInt(gridWidth / (radioStationsGrid.radioStationWidth + radioStationsGrid.horizontalSpacing))

            if(ceilItemCount >= core.radioStations.model.count){
                return radioStationsGrid.horizontalSpacing
            }

            const newSpacing = (gridWidth / ceilItemCount) - radioStationsGrid.radioStationWidth
            return newSpacing
        }

        anchors{
            fill: parent
        }

        cellHeight: radioStationsGrid.verticalSpacing + radioStationsGrid.radioStationHeight
        cellWidth: radioStationsGrid.currentHorizontalSpacing + radioStationsGrid.radioStationWidth

        model: core.radioStations.model
        delegate: Column{
            width: radioStationsGrid.cellWidth
            Rectangle{
                anchors{
                    horizontalCenter: parent.horizontalCenter
                }
                width: radioStationsGrid.radioStationWidth
                height: radioStationsGrid.radioStationHeight
                radius: 15
                color: "#C0C0C0"

                Image {
                    id: img
                    width: parent.width
                    height: parent.height
                    source: "data:image/jpg;base64," + model.image
                }

                Rectangle
                {
                    id: blureRects
                    anchors.centerIn: parent
                    width: 80
                    height: 80
                    color: "transparent"
                    radius: height / 2

                    Image{
                        id: playIcon

                        anchors{
                            centerIn: parent
                        }

                        opacity: 0

                        source: "qrc:/assets/icons/play.svg"

                        Behavior on opacity{
                            OpacityAnimator { duration: 150; easing.type: Easing.OutExpo }
                        }
                    }
                }

                MouseArea{
                    anchors{
                        fill: parent
                    }
                    hoverEnabled: true
                    onEntered:{
                        playIcon.opacity = 1
                    }
                    onExited:{
                        playIcon.opacity = 0
                    }
                    onClicked:{
                        console.warn("Clicked on radio station index:", index)
                    }
                }
            }
            Text{
                anchors{
                    horizontalCenter: parent.horizontalCenter
                }

                text: model.name
            }
        }
    }
}
