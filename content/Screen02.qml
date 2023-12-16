

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

    Item {

        id: mainLayout
        anchors.fill: parent

        Rectangle {
            id: circleLeft
            x: 5
            width: (parent.width<parent.height?parent.width:parent.height)
            height: width
            radius: width*0.5
            border.width: 0
            color: "#000000"
            visible: visible

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
            color: "#000000"
            visible: visible

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
            trackColor: "#e63129"
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
            progressColor: "#e63129"
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

    Item {
        id: __materialLibrary__
    }
}

/*##^##
Designer {
    D{i:0;formeditorColor:"#000000";formeditorZoom:0.125;height:400;width:1280}
}
##^##*/

