import QtQuick 2.15
import QtQuick.Controls 2.15
import Dashboard
import SpotifyWebApi

Item {

    id: root
    state: "dataUnavailable"

    property int pageIndex: 0

    Connections {
        target: rightButton
        onButtonPressedSignal: {
            if(SpotifyWebApi.dataAvailable && mainScreenView.screenIndex == 0)
            {
                pageIndex++
                pageIndex = pageIndex == 2 ? 0 : pageIndex
                if(pageIndex === 1)
                    loader.loadComponent(musicLyrics)
                else
                    loader.loadComponent(musicInfoAvailable)
            }
        }
    }

    Item {

        id: loader

        width: parent.width
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter

        property var sourceComponent
        function loadComponent(component){
            loader.sourceComponent = component;
            fade.start()
        }

        Column {
            id: column
            padding: 8
            spacing: 12
            width: parent.width
            anchors.verticalCenter: parent.verticalCenter

            Image {
                height: 24
                source: "images/Spotify_Logo_RGB_White.png"
                anchors.horizontalCenter: parent.horizontalCenter
                visible: SpotifyWebApi.dataAvailable
                fillMode: Image.PreserveAspectFit
            }

            Loader {
                id: internalLoader
                width: parent.width

                SequentialAnimation {

                    id: fade

                    NumberAnimation {
                        target: internalLoader
                        property: "opacity"
                        to: 0.0
                        duration: 200
                    }

                    ScriptAction {
                        script: {
                            if(SpotifyWebApi.dataAvailable)
                                carLayout.state = "variationFocus"
                            else
                                carLayout.state = "variation4Zoom"
                            internalLoader.sourceComponent = loader.sourceComponent
                        }
                    }

                    NumberAnimation {
                        target: internalLoader
                        property: "opacity"
                        to: 1.0
                        duration: 200
                    }

                }
            }

        }
    }


    Component {
        id: musicLyrics
        MusicLyrics {
            width: parent.width
            height: parent.height
            horizontalAlignment: Text.AlignHCenter
        }
    }

    Component {
        id: musicInfoAvailable
        Column {
            id: column1

            width: parent.width
            spacing: 12
            Image {
                id: albumCover
                height: 100
                width: 100
                source: SpotifyWebApi.songAlbumImage
                anchors.horizontalCenter: parent.horizontalCenter
                fillMode: Image.PreserveAspectFit
            }

            Column {
                width: parent.width
                spacing: 8

                Text {
                    color: "#ffffff"
                    text: "<b>"+SpotifyWebApi.songTitle+"</b>"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 22
                    wrapMode: Text.Wrap
                    width: parent.width

                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    maximumLineCount: 2
                }

                Text {
                    color: "#ffffff"
                    text: SpotifyWebApi.songArtists
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 18
                    wrapMode: Text.Wrap
                    width: parent.width

                    horizontalAlignment: Text.AlignHCenter
                    elide: Text.ElideRight
                    maximumLineCount: 2
                }

                ProgressBar {
                    id: progressBar

                    value: SpotifyWebApi.currentSongProgress
                    to: SpotifyWebApi.progressBarTotal
                    from: 0
                    width: 225
                    anchors.horizontalCenter: parent.horizontalCenter

                    background: Rectangle {
                        implicitWidth: progressBar.width
                        implicitHeight: 6
                        color: "transparent"
                    }

                    contentItem: Item {
                        implicitWidth: progressBar.width
                        implicitHeight: 6

                        Rectangle {
                            width: progressBar.visualPosition * parent.width
                            height: parent.height
                            radius: 0
                            color: "#ffffff"
                        }
                    }
                }
            }
        }
    }


    Component {
        id: musicInfoUnavailable
        Row {

            width: parent.width

            spacing: 12
            Column {

                width: parent.width

                Item {
                    height: 15
                    width: parent.width
                }

                Text {
                    color: "#ffffff"
                    text: "<b>Music for every moment</b>"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 16
                    wrapMode: Text.Wrap

                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                }


                Text {
                    color: "#ffffff"
                    text: "Sync Dashboard to your Spotify Account in the settings"
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 14
                    wrapMode: Text.Wrap

                    horizontalAlignment: Text.AlignHCenter
                    width: parent.width
                }

                Item {
                    height: 235
                    width: parent.width
                }
            }
        }
    }

    Item {
        id: __materialLibrary__
    }




    states: [
        State {
            name: "dataUnavailable"
            when: !SpotifyWebApi.dataAvailable
            StateChangeScript {
               script: {
                   loader.loadComponent(musicInfoUnavailable)
               }
            }
        },

        State {
            name: "dataAvailable"
            when: SpotifyWebApi.dataAvailable
            StateChangeScript {
               script: {
                   loader.loadComponent(musicInfoAvailable)
               }
            }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#808080";formeditorZoom:0.9;height:250;width:350}
}
##^##*/
