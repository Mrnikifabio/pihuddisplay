pragma Singleton
import QtQuick 6.2

QtObject {
    readonly property int width: 1280
    readonly property int height: 400

    property string relativeFontDirectory: "fonts"

    /* Edit this comment to add your custom font */
    readonly property font font: Qt.font({
                                             family: Qt.application.font.family,
                                             pixelSize: Qt.application.font.pixelSize
                                         })
    readonly property font largeFont: Qt.font({
                                                  family: Qt.application.font.family,
                                                  pixelSize: Qt.application.font.pixelSize * 1.6
                                              })

    readonly property color backgroundColor: "#c2c2c2"


    readonly property Gradient backgroundGradient: Gradient {

        GradientStop {
            position: 0
            color: "#000000"
        }
    }

    readonly property Transition pushEnter: Transition {
        NumberAnimation { property: "opacity"; from: 0; to: 1; duration: 200; easing.type: Easing.OutCubic }
    }
    readonly property Transition popExit: Transition {
        NumberAnimation { property: "opacity"; from: 1; to: 0; duration: 200; easing.type: Easing.OutCubic }
    }

    readonly property Transition pushExit: Transition {}
    readonly property Transition popEnter: Transition {}
}
