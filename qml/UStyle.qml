pragma Singleton

import QtQuick

QtObject {
    id: root

    // *** Colors ***
    // - Neutral -
    readonly property color white: Qt.color("#FFFFFF")          // lightest
    readonly property color veryLightGray: Qt.color("#F5F5F5") // very light gray
    readonly property color lightGray: Qt.color("#E0E0E0")     // light gray
    readonly property color silverGray: Qt.color("#C8C8C8")    // medium-light gray
    readonly property color mediumGray: Qt.color("#A0A0A0")    // medium gray
    readonly property color darkGray: Qt.color("#707070")      // dark gray
    readonly property color charcoalGray: Qt.color("#4A4A4A")  // darker gray
    readonly property color almostBlack: Qt.color("#1C1C1C")   // almost black
    readonly property color coalBlack: Qt.color("#101010")     // coal black
    readonly property color black: Qt.color("#000000")         // pure black

    // - Palette -
    readonly property color indigoBlue: Qt.color("#4465FC")
    readonly property color rosewood: Qt.color("#B84C4C")
    readonly property color crimsonRed: Qt.color("#E40D0D")



    // *** Neutral Colors ***
    property color neutral100: getNeutral100(core.styleIndex)
    property color neutral200: getNeutral200(core.styleIndex)
    property color neutral300: getNeutral300(core.styleIndex)
    property color neutral400: getNeutral400(core.styleIndex)
    property color neutral500: getNeutral500(core.styleIndex)
    property color neutral600: getNeutral600(core.styleIndex)
    property color neutral700: getNeutral700(core.styleIndex)
    property color neutral800: getNeutral800(core.styleIndex)
    property color neutral900: getNeutral900(core.styleIndex)



    // *** Functions to get colors based on style index (for future themes) ***
    function getNeutral100(styleIndex){
        if(styleIndex === 0){
            return root.white
        }
        else if (styleIndex === 1){
            return root.coalBlack
        }
    }
    function getNeutral200(styleIndex){
        if(styleIndex === 0){
            return root.veryLightGray
        }
        else if (styleIndex === 1){
            return root.almostBlack
        }
    }
    function getNeutral300(styleIndex){
        if(styleIndex === 0){
            return root.lightGray
        }
        else if (styleIndex === 1){
            return root.charcoalGray
        }
    }
    function getNeutral400(styleIndex){
        if(styleIndex === 0){
            return root.silverGray
        }
        else if (styleIndex === 1){
            return root.darkGray
        }
    }
    function getNeutral500(styleIndex){
        if(styleIndex === 0){
            return root.mediumGray
        }
        else if (styleIndex === 1){
            return root.mediumGray
        }
    }
    function getNeutral600(styleIndex){
        if(styleIndex === 0){
            return root.darkGray
        }
        else if (styleIndex === 1){
            return root.silverGray
        }
    }
    function getNeutral700(styleIndex){
        if(styleIndex === 0){
            return root.charcoalGray
        }
        else if (styleIndex === 1){
            return root.lightGray
        }
    }
    function getNeutral800(styleIndex){
        if(styleIndex === 0){
            return root.almostBlack
        }
        else if (styleIndex === 1){
            return root.veryLightGray
        }
    }
    function getNeutral900(styleIndex){
        if(styleIndex === 0){
            return root.coalBlack
        }
        else if (styleIndex === 1){
            return root.white
        }
    }



    // *** Fonst ***

    readonly property font primaryFontRegular10: Qt.font({ family: "Afacad", pixelSize: 10 })
    readonly property font primaryFontRegular12: Qt.font({ family: "Afacad", pixelSize: 12 })
    readonly property font primaryFontRegular14: Qt.font({ family: "Afacad", pixelSize: 14 })
    readonly property font primaryFontRegular16: Qt.font({ family: "Afacad", pixelSize: 16 })
    readonly property font primaryFontSemibold10: Qt.font({ family: "Afacad", pixelSize: 10, weight: Font.DemiBold })
    readonly property font primaryFontSemibold12: Qt.font({ family: "Afacad", pixelSize: 12, weight: Font.DemiBold })
    readonly property font primaryFontSemibold14: Qt.font({ family: "Afacad", pixelSize: 14, weight: Font.DemiBold })
    readonly property font primaryFontSemibold16: Qt.font({ family: "Afacad", pixelSize: 16, weight: Font.DemiBold })
    readonly property font primaryFontSemibold20: Qt.font({ family: "Afacad", pixelSize: 20, weight: Font.DemiBold })
    readonly property font primaryFontBold24: Qt.font({ family: "Afacad", pixelSize: 24, weight: Font.Bold })



    // *** Behaviors (for reusability) ***
    Behavior on neutral100{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral200{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral300{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral400{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral500{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral600{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral700{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral800{
        ColorAnimation{
            duration: 100
        }
    }
    Behavior on neutral900{
        ColorAnimation{
            duration: 100
        }
    }
}
