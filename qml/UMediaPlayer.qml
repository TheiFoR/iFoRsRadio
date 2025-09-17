import QtQuick
import QtQuick.Controls
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
        id: layoutRowLayout

        anchors{
            fill: parent
            topMargin: 8
            bottomMargin: 8
            leftMargin: 10
            rightMargin: 10
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
            Item {
                id: volumeWrapper
                width: controlsContainer.height
                height: width

                UIconButton {
                    id: volumeButton
                    anchors.fill: parent

                    iconColor: UStyle.neutral100
                    iconSource: {
                        if (core.mediaplayer.volume == 0) {
                            return "qrc:/assets/icons/sound_off.svg"
                        } else if (core.mediaplayer.volume < 0.33) {
                            return "qrc:/assets/icons/sound_min.svg"
                        } else if (core.mediaplayer.volume < 0.66) {
                            return "qrc:/assets/icons/sound_half.svg"
                        } else {
                            return "qrc:/assets/icons/sound.svg"
                        }
                    }

                    onClicked: core.mediaplayer.muted = !core.mediaplayer.muted
                }

                HoverHandler {
                    id: volumeHover
                    acceptedDevices: PointerDevice.Mouse
                    onHoveredChanged: {
                        if (hovered) {
                            volumeWrapper.showVolumeSlider()
                        } else {
                            volumeWrapper.hideVolumeSliderDelayed()
                        }
                    }
                }

                // --- Volume Slider ---
                Rectangle {
                    id: volumeSliderContainer

                    property bool open: false

                    opacity: open ? 1 : 0
                    visible: opacity > 0

                    anchors {
                        horizontalCenter: parent.horizontalCenter
                        bottom: parent.top
                        bottomMargin: layoutRowLayout.anchors.topMargin + 5
                    }

                    width: 12
                    height: 100
                    radius: width / 2
                    color: UStyle.neutral800

                    Slider {
                        id: volumeSlider
                        anchors{
                            fill: parent
                            topMargin: 5
                            bottomMargin: 5
                            leftMargin: 5
                            rightMargin: 5
                        }
                        orientation: Qt.Vertical

                        value: core.mediaplayer.volume

                        onValueChanged: {
                            if (volumeSlider.live) {
                                core.mediaplayer.volume = value
                                if(core.mediaplayer.volume == 0){
                                    core.mediaplayer.muted = true
                                }
                                else{
                                    core.mediaplayer.muted = false
                                }
                            }
                        }
                    }

                    Behavior on opacity {
                        OpacityAnimator { duration: 100 }
                    }

                    HoverHandler {
                        id: sliderHover
                        acceptedDevices: PointerDevice.Mouse
                        onHoveredChanged: {
                            if (hovered) {
                                volumeWrapper.showVolumeSlider()
                            } else {
                                volumeWrapper.hideVolumeSliderDelayed()
                            }
                        }
                    }

                    Timer {
                        id: hideVolumeSliderTimer
                        interval: 500
                        repeat: false
                        onTriggered: volumeSliderContainer.open = false
                    }
                }

                function showVolumeSlider() {
                    hideVolumeSliderTimer.stop()
                    volumeSliderContainer.open = true
                }

                function hideVolumeSliderDelayed() {
                    hideVolumeSliderTimer.restart()
                }
            }
        }
    }
}
