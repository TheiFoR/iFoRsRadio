import QtQuick
import QtQuick.Layouts
import QtQuick.Effects
import Qt5Compat.GraphicalEffects

import Enums

ColumnLayout {
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

        Layout.fillHeight: true
        Layout.fillWidth: true

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

            playState: model.state

            blinkAnimationOffset: -index * 3

            onClicked:{
                switch(model.state){
                    case PlayStates.Stopped:
                    case PlayStates.Paused:
                        core.radioStations.play(model.id)
                        break
                    case PlayStates.Playing:
                        core.radioStations.stop()
                        break
                }
            }
        }
    }

    UMediaPlayer{
        id: mediaplayerContainer

        Layout.fillHeight: true
        Layout.fillWidth: true

        Layout.maximumHeight: 40
        Layout.minimumHeight: 40
    }
}
