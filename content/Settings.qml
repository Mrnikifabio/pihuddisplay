import QtQuick 6.2
import QtQuick.Controls 6.2
import Dashboard
import QtQuick.Layouts 6.3
import QtQuick3D
import ButtonManager
import BluetoothManager
import SpotifyAuthManager
import Qt5Compat.GraphicalEffects

Item {

    property int scrollIndex: 0

    function handleScroll()
    {
        scrollIndex++
        scrollIndex = scrollIndex == 3 ? 0 : scrollIndex
        switch(scrollIndex)
        {
        case 0:
            infoLabel.border.width = 0
            unpairLabel.border.width = 5
            break;
        case 1:
            unpairLabel.border.width = 0
            spotifyLabel.border.width = 5
            break;
        case 2:
            spotifyLabel.border.width = 0
            infoLabel.border.width = 5
            break;
        }
    }

    function handleSelection()
    {
        switch(scrollIndex)
        {
        case 0:
            BluetoothManager.setDiscoverableAndBeginPairing()
            break;
        case 1:
            if(!SpotifyAuthManager.configured)
                SpotifyAuthManager.beginAuthProcess()
            else
                SpotifyAuthManager.clearSettings()
            break;
        }
    }

    Connections {
        target: rightButton
        onButtonPressedSignal: {
            handleScroll()
        }
    }

    Connections {
        target: middleButton
        onButtonPressedSignal: {
            handleSelection()
        }
    }

    Item {
        anchors.fill: parent
        DropShadow {
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
                    GradientStop { position: 0.0; color: "#ed8228" }
                    GradientStop { position: 0.49; color: "#000000" }
                    GradientStop { position: 0.5; color: "transparent" }
                }
            }
            color: "#000000"
            visible: visible
        }

        Column {
            padding: 12
            spacing: 18
            anchors.fill: parent

            Text {
                color: "#ffffff"
                text: "Settings"
                font.pointSize: 36
                font.bold: true
            }

            Rectangle {
                id: unpairLabel
                color: "transparent"
                border.color: "#ffffff"
                border.width: 5
                radius: 5
                width: childrenRect.width
                height: childrenRect.height

                Column {
                    padding: 8
                    Text {
                        text: BluetoothManager.paired ? "Unpair "+BluetoothManager.pairedDeviceName : "Pair a new device"
                        font.pointSize: 24
                        color: "#ffffff"
                    }

                    Text {
                        text: BluetoothManager.paired ? "Unpairing "+BluetoothManager.pairedDeviceName+" will quickly let you pair a new device" : "Pair a new device to access features on the Internet"
                        font.pointSize: 14
                        color: "#ffffff"
                    }

                }
            }

            Rectangle {
                id: spotifyLabel
                color: "transparent"
                border.color: "#ffffff"
                radius: 5
                border.width: 0
                width: childrenRect.width
                height: childrenRect.height


                Column {
                    padding: 8
                    Text {
                        text: SpotifyAuthManager.configured ? "Clear Spotify settings" : "Authenticate on Spotify"
                        color: "#ffffff"
                        font.pointSize: 24
                    }

                    Text {
                        text: SpotifyAuthManager.configured ? "Remove your account from this device" : "A spotify account is required to sync the multimedia page"
                        font.pointSize: 14
                        color: "#ffffff"
                    }
                }
            }

            Rectangle {
                id: infoLabel
                color: "transparent"
                border.color: "#ffffff"
                border.width: 0
                radius: 5
                width: childrenRect.width
                height: childrenRect.height

                Column {
                    padding: 8
                    Text {
                        text: "Info"
                        color: "#ffffff"
                        font.pointSize: 24
                    }

                    Text {
                        text: "Mrnikifabio 2022 & The Qt Company Framework\nThis project is not affiliated to VW (VolksWagen) in any way"
                        font.pointSize: 14
                        color: "#ffffff"
                    }
                }
            }

            Item {
                id: __materialLibrary__
            }
        }

    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#000000";formeditorZoom:0.75;height:400;width:1280}
}
##^##*/
