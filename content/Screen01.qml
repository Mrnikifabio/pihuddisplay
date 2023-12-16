

/*
This is a UI file (.ui.qml) that is intended to be edited in Qt Design Studio only.
It is supposed to be strictly declarative and only uses a subset of QML. If you edit
this file manually, you might introduce QML code that is not supported by Qt Design Studio.
Check out https://doc.qt.io/qtcreator/creator-quick-ui-forms.html for details on .ui.qml files.
*/
import QtQuick 6.2
import QtQuick.Controls 6.2
import Dashboard
import QtQuick.Layouts 6.3
import QtQuick3D
import Qt5Compat.GraphicalEffects
import SpeedExposer
import RPMExposer

Item {
    id: rectangle
    width: Constants.width
    height: Constants.height
    state: "empty state"
    readonly property int duration: 4000

    property string color: "#e63129"
    property string accent: "#e63129"

    property var colors: ["#e63129", "#1ed760", "#ed8228", "#7749be", "#000000"]
    property var accents: ["#e63129", "#1ed760", "#ed8228", "#7749be", "#000000"]

    property int colorIndex: 1
    property int accentIndex: 1

    function changeState()
    {
        switch(rectangle.state)
        {
            case "empty state":
                rectangle.state = "base state"
                break;
        }
    }


    Connections {
        target: middleButton
        onButtonPressedSignal: {
            fadeL.start()
            fadeR.start()
        }
    }

    Timer {
        id: stateTimer
        interval: 100
        onTriggered: rectangle.changeState()
    }

    Item {

        id: mainLayout
        anchors.fill: parent

        DropShadow {
            id: dropShadowRight
            anchors.fill: circleRight
            horizontalOffset: -4
            verticalOffset: 0
            radius: 30
            color: "#000000"
            source: circleRight
            samples: 17
        }

        DropShadow {
            id: dropShadowLeft
            anchors.fill: circleLeft
            horizontalOffset: 4
            verticalOffset: 0
            radius: 30
            color: "#000000"
            source: circleLeft
            samples: 17
        }

        Rectangle {
            id: circleLeft
            x: 5
            width: (parent.width<parent.height?parent.width:parent.height)
            height: width
            radius: width*0.5
            border.width: 0
            property double fillPosition : 0.0

            SequentialAnimation {
                id: fadeL
                NumberAnimation {
                    target: circleLeft
                    properties: "opacity"
                    to: 0.0
                    duration: 300
                }

                PauseAnimation {
                    duration: 100
                }

                ScriptAction {
                    script: {
                        rectangle.colorIndex = rectangle.colorIndex === 5 ? 0 : rectangle.colorIndex
                        rectangle.accentIndex = rectangle.accentIndex === 5 ? 0 : rectangle.accentIndex
                        rectangle.color = rectangle.colors[rectangle.colorIndex++]
                        rectangle.accent = rectangle.accents[rectangle.accentIndex++]
                    }
                }

                PauseAnimation {
                    duration: 100
                }

                NumberAnimation {
                    target: circleLeft
                    properties: "opacity"
                    to: 1.0
                    duration: 300
                }
            }

            SequentialAnimation {
                id: leftAnimation
                running: true

                PauseAnimation {
                    duration: 200

                }

                SequentialAnimation {

                    loops: Animation.Infinite

                    NumberAnimation {
                        properties: "fillPosition"
                        target: circleLeft
                        from: 0.0
                        to: 0.2
                        duration: rectangle.duration
                    }

                    NumberAnimation {
                        properties: "fillPosition"
                        target: circleLeft
                        from: 0.2
                        to: 0.0
                        duration: rectangle.duration
                    }
                }

            }

            RadialGradient {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: circleLeft.fillPosition; color: rectangle.color}
                    GradientStop { position: 0.49; color: "#000000" }
                    GradientStop { position: 0.5; color: "transparent" }
                }
            }
            color: "#000000"

            CruisingInfo {
                anchors.fill: parent
                alignment: Text.AlignHCenter
                text:"rpm"
                value: RPMExposer.currentValue
            }

        }

        Rectangle {
            id: circleRight
            x: 875
            width: (parent.width<parent.height?parent.width:parent.height) //*2.2
            height: width
            radius: width*0.5
            border.width: 0

            property double fillPosition : 0.0


            SequentialAnimation {
                id: fadeR
                NumberAnimation {
                    target: circleRight
                    properties: "opacity"
                    to: 0.0
                    duration: 300
                }

                PauseAnimation {
                    duration: 200
                }
                NumberAnimation {
                    target: circleRight
                    properties: "opacity"
                    to: 1.0
                    duration: 300
                }
            }

            SequentialAnimation {

                id: rightAnimation
                running: true

                PauseAnimation {
                    duration: 200
                }

                SequentialAnimation {

                    loops: Animation.Infinite

                    NumberAnimation {
                        properties: "fillPosition"
                        target: circleRight
                        from: 0.0
                        to: 0.2
                        duration: rectangle.duration
                    }

                    NumberAnimation {
                        properties: "fillPosition"
                        target: circleRight
                        from: 0.2
                        to: 0.0
                        duration: rectangle.duration
                    }
                }

            }

            RadialGradient {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: circleRight.fillPosition; color: rectangle.color }
                    GradientStop { position: 0.49; color: "#000000" }
                    GradientStop { position: 0.5; color: "transparent" }
                }
            }
            color: "#000000"

            CruisingInfo {
                anchors.fill: parent
                alignment: Text.AlignHCenter
                text:"km/h"
                value: SpeedExposer.currentValue
            }
        }


        CircularSlider {
            x: 5
            width: (parent.width<parent.height?parent.width:parent.height)
            height: width
            endAngle: 110
            startAngle: -40
            trackColor: rectangle.accent
            progressColor: "#505050"
            value: 7000-RPMExposer.currentValue
            minValue: 0
            maxValue: 7000

        }

        CircularSlider {
            x: 875
            width: (parent.width<parent.height?parent.width:parent.height)
            height: width
            startAngle: -110
            endAngle: 40
            progressColor: rectangle.accent
            minValue: 0
            maxValue: 240
            value: SpeedExposer.currentValue
        }




        Column {
            anchors.fill: parent

            Row {
                id: row
                width: parent.width
                height: parent.height * 0.1

                Clock {
                    id: clock
                    height: parent.height
                    width: parent.width
                }
            }

            Row {
                id: row1
                width: parent.width
                height: parent.height*0.9

                Item {
                    width: parent.width * 0.34
                    height: parent.height
                }

                MusicInfo {
                    width: parent.width * 0.32
                    height: parent.height
                }


                Item {
                    width: parent.width * 0.34
                    height: parent.height
                }
            }
        }

    }

    states: [
        State {
            name: "empty state"

            PropertyChanges {
                target: mainLayout
                opacity: 0
            }

            PropertyChanges {
                target: stateTimer
                running: true
            }
        }
    ]

    transitions: [
        Transition {
            id: emptyToBaseTransition
            ParallelAnimation {
                PropertyAnimation {
                    target: mainLayout
                    property: "opacity"
                    duration: 1600
                }
            }
            from: "empty state"
        }
    ]

    Item {
        id: __materialLibrary__
    }
}

/*##^##
Designer {
    D{i:0;formeditorColor:"#000000";formeditorZoom:0.125;height:400;width:1280}
}
##^##*/

