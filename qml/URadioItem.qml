import QtQuick
import QtQuick.Controls.impl
import QtQuick.Effects
import Qt5Compat.GraphicalEffects

import UStyle
import Enums

Item{
    id: root

    property alias radius: radioItemBackground.radius
    property alias source: radioImage.source
    property alias title: radioTitle.text
    property alias genre: radioGenre.text

    property int playState: PlayStates.Stopped

    property int blinkAnimationOffset: 0

    readonly property bool ready: radioImage.progress === 1.0

    signal clicked()

    // *** Private properties ***
    property int __loadingTick: root.blinkAnimationOffset / (loadAnimationTimer.interval / 20)

    readonly property int __blinkAnimationDuration: 600
    readonly property int __loadingAnimationDuration: 1000
    readonly property int __imageAnimationDuration: 300
    readonly property int __textMoveAnimationDuration: 300
    readonly property int __textOpacityAnimationDuration: 600
    readonly property int __playIconAnimationDuration: 100

    Timer{
        id: loadAnimationTimer

        running: !root.ready
        interval: 20
        repeat: true
        onTriggered: {
            root.__loadingTick = root.__loadingTick + 1
        }
    }

    Column {
        id: radioItemContainer

        anchors{
            fill: parent
        }

        Rectangle{
            id: radioItemBackground

            z: titleContainer.z + 1

            color: {
                if (ready) {
                    return UStyle.neutral800
                }

                // cubic triangle wave in [0..1]
                function trianglePulse(x) {
                    var t = x % 2.0;
                    var v = 1.0 - Math.abs(t - 1.0);
                    return Math.pow(v, 3);
                }

                // map wave to [1.0 .. 1.5]
                var wave = trianglePulse(root.__loadingTick / (root.__blinkAnimationDuration / loadAnimationTimer.interval));
                var factor = 1.0 + wave * 0.5;

                return Qt.lighter(UStyle.neutral800, factor);
            }

            width: radioItemContainer.width
            height: width

            radius: 10

            //Add an opacity mask with a radius to the layer
            layer.enabled: true
            layer.smooth: true
            layer.effect: OpacityMask {
                source: radioItemBackground
                maskSource: Rectangle {
                    width: radioItemBackground.width
                    height: radioItemBackground.height
                    radius: radioItemBackground.radius
                    color: "white"
                }
            }

            Image{
                id: radioImage

                anchors{
                    fill: parent
                }

                fillMode: Image.PreserveAspectCrop
                source: ""

                opacity: root.ready ? 1.0 : 0.0

                Behavior on opacity{
                    OpacityAnimator{
                        duration: root.__imageAnimationDuration
                    }
                }
            }

            // Blurred circular rectangle
            Rectangle {
                id: glassRect

                visible: root.ready

                anchors{
                    centerIn: parent
                }

                width: 40
                height: 40

                radius: width / 2
                color: "#27FFFFFF"

                FastBlur {
                    id: blurEffect
                    anchors.fill: parent
                    source: ShaderEffectSource {
                        sourceItem: radioImage
                        recursive: true
                        sourceRect: Qt.rect(glassRect.x, glassRect.y, glassRect.width, glassRect.height)
                    }
                    radius: mouseArea.containsMouse ? 64 : 32

                    Behavior on radius{
                        NumberAnimation{
                            duration: root.__playIconAnimationDuration
                        }
                    }
                }

                layer.enabled: true
                layer.effect: OpacityMask {
                    maskSource: Rectangle {
                        width: glassRect.width
                        height: glassRect.height
                        radius: glassRect.radius
                        color: "white"
                    }
                }

                border.color: UStyle.neutral300
                border.width: 1

                Item{
                    id: playIconContainer

                    anchors{
                        centerIn: parent
                    }

                    Row {
                        id: dots

                        visible: root.playState === PlayStates.Loading

                        spacing: 4
                        anchors.centerIn: parent

                        Repeater {
                            id: dotsRepeater
                            model: 3

                            Rectangle {
                                id: dot
                                width: 8
                                height: 8

                                radius: width / 2
                                color: UStyle.neutral100
                                opacity: 1

                                SequentialAnimation on y{
                                    running: dots.visible
                                    loops: Animation.Infinite
                                    PauseAnimation{ duration: root.__loadingAnimationDuration / 6 * index }
                                    NumberAnimation{ from: 0; to: -dot.height / 2; duration: root.__loadingAnimationDuration / 2; easing.type: Easing.InOutSine }
                                    NumberAnimation{ from: -dot.height / 2; to: 0; duration: root.__loadingAnimationDuration / 2; easing.type: Easing.InOutSine }
                                    PauseAnimation{ duration: root.__loadingAnimationDuration / 6 * dotsRepeater.model - root.__loadingAnimationDuration / 6 * index }
                                }
                            }
                        }
                    }

                    ColorImage{
                        id: iconImage

                        visible: root.playState !== PlayStates.Loading

                        anchors{
                            centerIn: parent
                        }

                        source: {
                            if (root.playState === PlayStates.Loading) {
                                return "qrc:/assets/icons/loading.svg"
                            } else if (root.playState === PlayStates.Playing) {
                                return "qrc:/assets/icons/pause.svg"
                            } else if (root.playState === PlayStates.Stopped || root.playState === PlayStates.Paused) {
                                return "qrc:/assets/icons/play.svg"
                            }
                        }

                        width: 24
                        height: 24

                        sourceSize{
                            width: iconImage.width
                            height: iconImage.height
                        }

                        fillMode: Image.PreserveAspectFit

                        layer.enabled: true
                        layer.effect: MultiEffect {
                            shadowEnabled: true
                            shadowColor: "#40000000"   // чёрный с opacity ~25%
                            shadowVerticalOffset: 0
                            shadowHorizontalOffset: 0
                            shadowBlur: 0.4            // лёгкий размыв
                        }
                    }
                }
            }
        }

        Item{
            id: titleContainer

            anchors{
                left: parent.left
                right: parent.right
            }

            z: genreeContainer.z + 1

            height: radioTitle.height

            Text{
                id: radioTitle

                anchors{
                    top: parent.top
                    left: parent.left
                    right: parent.right

                    topMargin: root.ready ? 0 : -10
                }

                opacity: root.ready ? 1 : 0

                text: "Unknown"
                wrapMode: Text.WordWrap

                font: UStyle.primaryFontSemibold14
                color: UStyle.neutral100

                Behavior on opacity{
                    OpacityAnimator{
                        duration: root.__textOpacityAnimationDuration
                    }
                }

                Behavior on anchors.topMargin {
                    SmoothedAnimation{
                        velocity: -1
                        duration: root.__textMoveAnimationDuration
                    }
                }
            }
        }

        Item{
            id: genreeContainer

            anchors{
                left: parent.left
                right: parent.right
            }

            height: radioGenre.height

            Text{
                id: radioGenre

                anchors{
                    top: parent.top
                    left: parent.left
                    right: parent.right

                    topMargin: root.ready ? 0 : -15
                }

                opacity: root.ready ? 1 : 0

                text: "Unknown"
                wrapMode: Text.WordWrap

                font: UStyle.primaryFontSemibold12
                color: UStyle.neutral400

                Behavior on opacity{
                    OpacityAnimator{
                        duration: root.__textOpacityAnimationDuration
                    }
                }

                Behavior on anchors.topMargin {
                    SmoothedAnimation{
                        velocity: -1
                        duration: root.__textMoveAnimationDuration
                    }
                }
            }
        }
    }

    MouseArea{
        id: mouseArea

        enabled: root.ready

        hoverEnabled: true

        anchors{
            fill: parent
        }

        onClicked:{
            root.clicked()
        }
    }
}
