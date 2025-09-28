import QtQuick
import QtQuick.Layouts

import Qt5Compat.GraphicalEffects

import Enums
import UStyle

UWindow {
    id: root

    width: 880
    height: 550

    minimumWidth: 700
    minimumHeight: 450

    title: qsTr("iFoRsRadio")

    FontLoader {
        id: afacadRegularFont
        source: "qrc:/assets/font/Afacad/Afacad-Medium.ttf"
    }
    FontLoader {
        id: afacadBoldFont
        source: "qrc:/assets/font/Afacad/Afacad-Bold.ttf"
    }

    Item {
        id: rootOpacityContainer

        anchors.fill: parent

        layer.enabled: true
        layer.smooth: true

        layer.effect: OpacityMask {
            source: rootOpacityContainer
            maskSource: Rectangle {
                width: root.width
                height: root.height
                radius: root.radius
                color: "white"
            }
        }

        RowLayout{
            id: rootContainer

            anchors{
                fill: parent
            }

            spacing: 20

            Rectangle{
                id: menuPanel

                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.maximumWidth: leftSideContainer.childrenRect.width + leftSideContainer.anchors.margins * 2
                Layout.minimumWidth: leftSideContainer.childrenRect.width + leftSideContainer.anchors.margins * 2

                color: UStyle.neutral800

                ColumnLayout{
                    id: leftSideContainer

                    anchors{
                        top: parent.top
                        bottom: parent.bottom
                        left: parent.left
                        margins: 10
                    }

                    spacing: 10

                    LogoPanel{
                        id: logoPanel

                        Layout.fillWidth: true
                        Layout.fillHeight: true

                        Layout.maximumHeight: 30
                    }

                    ColumnLayout{
                        id: navigationContainer

                        spacing: 10

                        NavigationPanel{
                            id: navigationPanel

                            Layout.fillWidth: true
                            Layout.fillHeight: false

                            spacing: 10
                        }

                        Item{
                            Layout.fillWidth: true
                            Layout.fillHeight: true
                        }

                        Rectangle{
                            Layout.fillWidth: true

                            implicitHeight: 40
                            radius: 15

                            color: "transparent"
                            border{
                                color: themeSwitcherArea.containsMouse
                                       ? UStyle.neutral600
                                       : UStyle.neutral700
                                width: 1

                                Behavior on color{
                                    ColorAnimation{
                                        duration: 100
                                    }
                                }
                            }

                            RowLayout{
                                id: layoutRowLayout

                                anchors{
                                    fill: parent
                                    topMargin: 9
                                    bottomMargin: 9
                                    leftMargin: 20
                                    rightMargin: 20
                                }

                                Text{
                                    Layout.alignment: Qt.AlignVCenter
                                    text: "Theme"
                                    font: UStyle.primaryFontRegular16
                                    color: UStyle.neutral100
                                }

                                Item{
                                    Layout.fillWidth: true
                                    Layout.fillHeight: true
                                }

                                UThemeSwitcher{
                                    id: themeSwitcher

                                    Layout.fillWidth: false
                                    Layout.fillHeight: true

                                    Layout.minimumWidth: 38

                                    currentIndex: core.styleIndex
                                }
                            }

                            MouseArea{
                                id: themeSwitcherArea

                                anchors.fill: parent

                                cursorShape: Qt.PointingHandCursor
                                hoverEnabled: true

                                onClicked:{
                                    core.styleIndex = (core.styleIndex === 0 ? 1 : 0)
                                }
                            }
                        }
                    }
                }
            }
            ColumnLayout{
                id: pagesContainer

                Layout.fillWidth: true
                Layout.fillHeight: true

                Layout.rightMargin: 10
                Layout.bottomMargin: 10

                StackLayout{
                    id: pagesLayout

                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    Layout.topMargin: root.headerHeight

                    currentIndex: core.currentPage

                    RadioStationPage{}
                    RoomsPage{}
                    SettingsPage{}
                    AboutPage{}
                }
            }
        }
    }
}
