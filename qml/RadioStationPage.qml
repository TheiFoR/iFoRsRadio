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
        property int radioStationHeight: 150

        property real currentHorizontalSpacing:{
            const gridWidth = radioStationsGrid.width
            const ceilItemCount = parseInt(gridWidth / (radioStationsGrid.radioStationWidth + radioStationsGrid.horizontalSpacing))

            if(ceilItemCount >= core.radioStations.proxyModel.count){
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

        model: core.radioStations.proxyModel
        delegate: URadioItem{
            id: delegateRoot

            width: radioStationsGrid.radioStationWidth
            height: radioStationsGrid.radioStationHeight

            source: model.image
            title: model.name
            genre: model.genre

            blinkAnimationOffset: -index * 3

            onClicked:{
                core.radioStations.play(model.id)
            }
        }

        // delegate: Column{
        //     width: radioStationsGrid.cellWidth
        //     Rectangle{
        //         anchors{
        //             horizontalCenter: parent.horizontalCenter
        //         }
        //         width: radioStationsGrid.radioStationWidth
        //         height: radioStationsGrid.radioStationHeight
        //         radius: 15
        //         color: UStyle.neutral800

        //         Image {
        //             id: img
        //             width: parent.width
        //             height: parent.height
        //             source: model.image
        //             fillMode: Image.PreserveAspectCrop
        //         }

        //         Rectangle
        //         {
        //             id: blureRects
        //             anchors.centerIn: parent
        //             width: 80
        //             height: 80
        //             color: "transparent"
        //             radius: height / 2

        //             Image{
        //                 id: playIcon

        //                 anchors{
        //                     centerIn: parent
        //                 }

        //                 opacity: 0

        //                 source: "qrc:/assets/icons/play.svg"

        //                 Behavior on opacity{
        //                     OpacityAnimator { duration: 150; easing.type: Easing.OutExpo }
        //                 }
        //             }
        //         }

        //         MouseArea{
        //             anchors{
        //                 fill: parent
        //             }
        //             hoverEnabled: true
        //             onEntered:{
        //                 playIcon.opacity = 1
        //             }
        //             onExited:{
        //                 playIcon.opacity = 0
        //             }
        //             onClicked:{
        //                 console.warn(model["name"])
        //                 core.radioStations.play(model.id)
        //             }
        //         }
        //     }
        //     Text{
        //         anchors{
        //             horizontalCenter: parent.horizontalCenter
        //         }

        //         text: model.name
        //     }
        // }
    }
}
