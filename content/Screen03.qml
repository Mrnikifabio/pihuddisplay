

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

Item {
    id: rectangle
    width: Constants.width
    height: Constants.height
    readonly property int duration: 4000

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
            x: -355
            y: 40
            width: (parent.width<parent.height?parent.width:parent.height)*2.2
            height: width
            radius: width*0.5
            border.width: 0
            RadialGradient {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#1ed760" }
                    GradientStop { position: 0.49; color: "#000000" }
                    GradientStop { position: 0.5; color: "transparent" }
                }
            }

            SequentialAnimation {

                running: true

                PauseAnimation {
                    duration: 200
                }

                ParallelAnimation {
                    loops: Animation.Infinite
                    SequentialAnimation {

                        NumberAnimation {
                            properties: "x"
                            target: circleLeft
                            from: -300; to: -400
                            duration: rectangle.duration
                        }

                        NumberAnimation {
                            target: circleLeft
                            properties: "x"
                            from: -400; to: -300
                            duration: rectangle.duration
                        }
                    }

                    SequentialAnimation {
                        NumberAnimation {
                            target: circleLeft
                            properties: "y"
                            from: 0; to: 80
                            duration: rectangle.duration
                        }
                        NumberAnimation {
                            target: circleLeft
                            properties: "y"
                            from: 80; to: 0
                            duration: rectangle.duration
                        }
                    }
                }
            }


            color: "#000000"
            visible: visible
        }

        Rectangle {
            id: circleRight
            x: 780
            y: 40
            width: (parent.width<parent.height?parent.width:parent.height)*2.2
            height: width
            radius: width*0.5
            border.width: 0
            RadialGradient {
                anchors.fill: parent
                gradient: Gradient {
                    GradientStop { position: 0.0; color: "#1ed760" }
                    GradientStop { position: 0.49; color: "#000000" }
                    GradientStop { position: 0.5; color: "transparent" }
                }
            }
            color: "#000000"
            visible: visible


            SequentialAnimation {

                running: true

                PauseAnimation {
                    duration: 400
                }

                ParallelAnimation {
                    loops: Animation.Infinite
                    SequentialAnimation {

                        NumberAnimation {
                            target: circleRight
                            properties: "x"
                            from: 730; to: 830
                            duration: rectangle.duration
                        }
                        NumberAnimation {
                            target: circleRight
                            properties: "x"
                            from: 830; to: 730
                            duration: rectangle.duration
                        }
                    }

                    SequentialAnimation {
                        NumberAnimation {
                            target: circleRight
                            properties: "y"
                            from: 0; to: 80
                            duration: rectangle.duration
                        }
                        NumberAnimation {
                            target: circleRight
                            properties: "y"
                            from: 80; to: 0
                            duration: rectangle.duration
                        }
                    }
                }
            }

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
                rightPadding: 8
                leftPadding: 12

                MusicInfo {
                    width: parent.width * 0.4
                    height: parent.height
                    anchors.leftMargin: 0
                }


                Item {
                    width: parent.width * 0.2
                    height: parent.height
                }

                MusicLyrics {
                    width: parent.width * 0.4
                    height: parent.height
                    anchors.rightMargin: 0
                }
            }
        }

    }

    Item {
        id: __materialLibrary__
    }
}

/*##^##
Designer {
    D{i:0;formeditorColor:"#000000";formeditorZoom:0.5;height:400;width:1280}
}
##^##*/

