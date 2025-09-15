pragma Singleton

import QtQuick

QtObject {
    id: root

    // *** Colors ***
    // - Neutral -
    readonly property color white: Qt.color("#FFFFFF")
    readonly property color veryLightGray: Qt.color("#E9E9E9")
    readonly property color lightGray: Qt.color("#D4D4D4")
    readonly property color silverGray: Qt.color("#B0B0B0")
    readonly property color mediumGray: Qt.color("#8A8A8A")
    readonly property color darkGray: Qt.color("#616161")
    readonly property color charcoalGray: Qt.color("#464646")
    readonly property color almostBlack: Qt.color("#2B2B2B")
    readonly property color coalBlack: Qt.color("#101010")
    readonly property color black: Qt.color("#000000")
    // - Palette -
    readonly property color indigoBlue: Qt.color("#4465FC")
    readonly property color rosewood: Qt.color("#B84C4C")
    readonly property color crimsonRed: Qt.color("#E40D0D")



    // *** Neutral Colors ***
    readonly property color neutral100: getNeutral100(core.styleIndex)
    readonly property color neutral200: getNeutral200(core.styleIndex)
    readonly property color neutral300: getNeutral300(core.styleIndex)
    readonly property color neutral400: getNeutral400(core.styleIndex)
    readonly property color neutral500: getNeutral500(core.styleIndex)
    readonly property color neutral600: getNeutral600(core.styleIndex)
    readonly property color neutral700: getNeutral700(core.styleIndex)
    readonly property color neutral800: getNeutral800(core.styleIndex)
    readonly property color neutral900: getNeutral900(core.styleIndex)



    // *** Functions to get colors based on style index (for future themes) ***
    function getNeutral100(styleIndex){
        if(styleIndex === 0){
            return root.white
        }
    }
    function getNeutral200(styleIndex){
        if(styleIndex === 0){
            return root.veryLightGray
        }
    }
    function getNeutral300(styleIndex){
        if(styleIndex === 0){
            return root.lightGray
        }
    }
    function getNeutral400(styleIndex){
        if(styleIndex === 0){
            return root.silverGray
        }
    }
    function getNeutral500(styleIndex){
        if(styleIndex === 0){
            return root.mediumGray
        }
    }
    function getNeutral600(styleIndex){
        if(styleIndex === 0){
            return root.darkGray
        }
    }
    function getNeutral700(styleIndex){
        if(styleIndex === 0){
            return root.charcoalGray
        }
    }
    function getNeutral800(styleIndex){
        if(styleIndex === 0){
            return root.almostBlack
        }
    }
    function getNeutral900(styleIndex){
        if(styleIndex === 0){
            return root.coalBlack
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
}
