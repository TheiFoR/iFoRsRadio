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

        Text {
            id: textItem
            text: {
                if(core.mediaplayer.currentTrack.artist !== ""){
                    return core.mediaplayer.currentTrack.artist + " - " + core.mediaplayer.currentTrack.title
                }
                return core.mediaplayer.currentTrack.title
            }
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
                iconColor: UStyle.neutral300
                iconSource:{
                    switch(core.mediaplayer.currentState){
                        case PlayStates.Stopped:
                        case PlayStates.Paused:
                            return "qrc:/assets/icons/play.svg"
                        case PlayStates.Playing:
                            return "qrc:/assets/icons/pause.svg"
                        // case PlayStates.Loading:
                        //     return "qrc:/assets/icons/cloud.svg"
                        // case PlayStates.Error:
                        //     return "qrc:/assets/icons/error.svg"
                        // case PlayStates.Empty:
                        //     return "qrc:/assets/icons/play.svg"
                    }
                }

                onClicked:{
                    switch(core.mediaplayer.currentState){
                        case PlayStates.Stopped:
                        case PlayStates.Paused:
                            core.mediaplayer.play()
                            break
                        case PlayStates.Playing:
                            core.mediaplayer.stop()
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

                    iconColor: UStyle.neutral300
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

                    width: 16
                    height: 100
                    radius: width / 2
                    color: UStyle.neutral800

                    USlider {
                        id: volumeSlider

                        anchors.fill: parent
                        orientation: Qt.Vertical

                        from: 0
                        to: 1
                        stepSize: 0.01

                        // установка value, когда меняется громкость извне
                        Connections {
                            target: core.mediaplayer
                            function onVolumeChanged(vol) {
                                if (!volumeSlider.pressed) {
                                    volumeSlider.value = core.mediaplayer.volume
                                }
                            }
                        }

                        // изменение громкости, когда двигаем слайдер
                        onValueChanged: {
                            if (pressed) {
                                core.mediaplayer.volume = value
                                core.mediaplayer.muted = (value === 0)
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
