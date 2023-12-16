import QtQuick 2.15
import QtQuick.Controls 2.15
import SpeedExposer 1.0

Item {

    id: root
    property real alignment;
    property string text;
    property int value;

    Column {
        id: column
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        height: t1.height + t2.height
        width: parent.width

        Text {
            id: t1
            color: "#ffffff"
            text: "<b>"+root.value+"</b>"
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            wrapMode: Text.Wrap
            clip: true
            font.pointSize: 52
        }

        Text {
            id: t2
            color: "#ffffff"
            text: root.text
            wrapMode: Text.Wrap
            anchors.bottomMargin: 0
            horizontalAlignment: Text.AlignHCenter
            width: parent.width
            clip: true
            font.pointSize: 24
        }

    }

    Item {
        id: __materialLibrary__
    }

    Behavior on value {
        NumberAnimation {
            alwaysRunToEnd: true
            duration: 250
        }
    }
}
