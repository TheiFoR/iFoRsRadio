import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Basic

Slider {
    id: volumeSlider
    from: 0
    to: 1
    stepSize: 0.01
    value: 0.5

    width: 12
    height: 100
    orientation: Qt.Vertical

    // --- Background line ---
    background: Rectangle {
        id: backgroundRect
        anchors.fill: parent
        radius: width / 2
        color: "#333333"
    }

    // --- Filled part ---
    Rectangle {
        width: parent.width
        height: parent.availableHeight * ((volumeSlider.value - volumeSlider.from) / (volumeSlider.to - volumeSlider.from))
        anchors.bottom: parent.bottom
        radius: width / 2
        color: "#00AAFF"
    }

    // --- Handle ---
    handle: Rectangle {
        implicitWidth: 14
        implicitHeight: 14
        radius: width / 2
        color: "#FFFFFF"
        border.color: "transparent"

        // позиционируем в зависимости от value
        y: volumeSlider.height * (1 - (volumeSlider.position)) - height / 2
        x: (volumeSlider.width - width) / 2
    }
}

