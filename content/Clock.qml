import QtQuick 2.15
import QtQuick.Controls 2.15
import ClockExposer 1.0
import BluetoothManager 1.0

Item {
    id: item1

    QtObject {
        id: internal
        property int hour: Qt.formatDateTime(new Date(), "hh")
        property int minutes: Qt.formatDateTime(new Date(), "mm")
    }

    function formatNumber(number)
    {
        if(number < 10)
            return "0"+number;
        else
            return number+"";
    }

    Row {
        id: row
        anchors.verticalCenterOffset: 12
        anchors.horizontalCenterOffset: 12
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter

        Text {
            color: "#ffffff"
            text: formatNumber(ClockExposer.currentHour)+"<b>:</b>"+formatNumber(ClockExposer.currentMinute)
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 24
        }

        Item {
            width: 12
            height: 42
        }

        Image {
            source: "images/bluetooth.png"
            height: 28
            fillMode: Image.PreserveAspectFit
            visible: BluetoothManager.connected
            anchors.verticalCenter: parent.verticalCenter
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
