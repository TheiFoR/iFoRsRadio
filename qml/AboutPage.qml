import QtQuick
import QtQuick.Controls
import QtQuick.Controls.impl
import QtQuick.Layouts

Item {
    id: aboutPage

    ColumnLayout {
        id: content

        anchors{
            centerIn: parent
        }

        spacing: 20
        width: Math.min(parent.width * 0.8, 500)

        Text {
            text: "About"
            font: UStyle.primaryFontBold24
            color: UStyle.neutral100
            Layout.alignment: Qt.AlignHCenter
        }

        Image {
            Layout.alignment: Qt.AlignHCenter

            Layout.maximumHeight: 100
            Layout.minimumHeight: 100
            Layout.maximumWidth: 100
            Layout.minimumWidth: 100

            sourceSize{
                width: 100
                height: 100
            }

            source: "qrc:/assets/icons/app/icon.svg"
        }

        Column {
            Layout.alignment: Qt.AlignHCenter
            spacing: 4
            Text {
                anchors{
                    horizontalCenter: parent.horizontalCenter
                }

                text: core.appName
                font: UStyle.primaryFontSemibold20
                color: UStyle.neutral200
            }
            Text {
                anchors{
                    horizontalCenter: parent.horizontalCenter
                }

                text: "v" + core.appVersion
                font: UStyle.primaryFontRegular14
                color: UStyle.neutral500
            }
        }

        Text {
            text: core.appName + " is a modern and lightweight radio player that allows you to discover, listen and enjoy your favorite radio stations with ease."
            font: UStyle.primaryFontRegular14
            color: UStyle.neutral300
            wrapMode: Text.WordWrap
            horizontalAlignment: Text.AlignHCenter
            Layout.fillWidth: true
        }

        Text {
            text: "© 2025 iFoR. All rights reserved."
            font: UStyle.primaryFontRegular12
            color: UStyle.neutral600
            horizontalAlignment: Text.AlignHCenter
            Layout.alignment: Qt.AlignHCenter
        }
    }
}
