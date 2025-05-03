import QtQuick
import QtQuick.Layouts

Item {
    id: radioStationsPageRoot

    GridLayout{
        id: radioStationsGrid

        Repeater{
            model: core.radioStations.model
            delegate: Rectangle{

            }
        }
    }
}
