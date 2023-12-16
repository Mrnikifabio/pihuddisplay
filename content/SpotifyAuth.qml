import QtQuick 6.2
import QtQuick.Layouts 1.3
import ButtonManager
import SpotifyAuthManager
import QrImage
import QrImageProvider

Item {

    Connections {
        target: middleButton
        onButtonPressedSignal: {
            SpotifyAuthManager.endAuthProcess();
        }
    }

    Column {
        padding: 12
        spacing: 18
        anchors.fill: parent

        Text {
            color: "#ffffff"
            text: "Authenticate on your phone"
            font.pointSize: 36
            font.bold: true
        }

        Rectangle {
            id: spotifyLabel
            color: "transparent"
            width: parent.width
            height: childrenRect.height


            Row {
                width: parent.width
                spacing: 12
                padding: 8

                Text {
                    text: "Please, scan the qr code with your phone and follow the instructions over there.<br>After the procedure completes, the device will automatically sync.<br>You can press ok if you want to cancel the operation."
                    font.pointSize: 14
                    color: "#ffffff"
                }

                Item {
                    width: 125
                    height: 1
                }

                QrImage {
                    height: 200
                    width: 200
                    m_image: QrImageProvider.image
                }

                Item {
                    width: 125
                    height: 1
                }
            }
        }

        Item {
            id: __materialLibrary__
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.33;height:1;width:1280}
}
##^##*/
