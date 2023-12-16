import QtQuick 6.2
import QtQuick.Controls 6.2
import Dashboard
import QtQuick.Layouts 6.3
import BluetoothManager

Rectangle {
    gradient: Constants.backgroundGradient

    Connections {
        target: middleButton
        onButtonPressedSignal: {
            BluetoothManager.cancelPairing()
        }
    }

    Connections {
        target: BluetoothManager
        onConnectingToNAPAfterPairingSignal: {
            pairStatus.text = "Connecting to "+BluetoothManager.pairedDeviceName+"..."
        }
    }

    ColumnLayout {

        anchors.centerIn: parent

        Text {
            id: pairStatus
            text: "Please connect your phone via Bluetooth to 'Dashboard'"
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 18
            color: "#ffffff"
            horizontalAlignment: Text.AlignRight
            wrapMode: Text.Wrap
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
