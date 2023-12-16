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
import SpeedExposer

Item {
    id: rectangle
    width: Constants.width
    height: Constants.height

    Item {

        id: mainLayout
        anchors.fill: parent

        Column {
            anchors.fill: parent

            Row {
                id: row
                width: parent.width
                height: parent.height * 0.1
            }

            Row {
                id: row1
                width: parent.width
                height: parent.height*0.9
                rightPadding: 8
                leftPadding: 12

                Item {
                    width: parent.width * 0.2
                    height: parent.height
                }

                MusicLyrics {
                    width: parent.width * 0.6
                    height: parent.height
                    anchors.rightMargin: 0
                    horizontalAlignment: Text.AlignHCenter
                }


                Item {
                    width: parent.width * 0.2
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
    D{i:0;formeditorZoom:0.33;height:400;width:1280}
}
##^##*/

