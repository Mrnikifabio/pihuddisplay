import QtQuick 2.15
import QtQuick.Controls 2.15
import SpotifyLyricsApi
import SpotifyWebApi
import SpotifyAuthManager

Item {

    id: root
    property int horizontalAlignment: Text.AlignRight
    state: "dataUnavailable"

    function lyricsUnavailableString()
    {
        if(SpotifyAuthManager.authenticated)
        {
            if(SpotifyWebApi.dataAvailable)
                return "We can't fetch the lyric for the current song"
            else
                return "Nothing playing on Spotify"
        }
        else
            return "Nothing playing on Spotify"
    }

    Loader {
        id: loader
        anchors.fill: parent
        onSourceChanged: animation.running = true
        SequentialAnimation {
            PropertyAnimation {
                target: loader.item
                property: "opacity"
                from: 1
                to: 0
                duration: 800
            }
            PropertyAnimation {
                target: loader.item
                property: "opacity"
                from: 0
                to: 1
                duration: 800
            }
        }
    }

    Component {
        id: lyricsUnavailable
        Text {
            color: "#ffffff"
            anchors.fill: parent
            horizontalAlignment: root.horizontalAlignment
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap
            rightPadding: 24
            clip: true
            font.pointSize: 26
            text: "<b>"+lyricsUnavailableString()+"</b>"
        }
    }

    Component {
        id: lyricsAvailable
        Text {
            color: "#ffffff"
            anchors.fill: parent
            horizontalAlignment: root.horizontalAlignment
            verticalAlignment: Text.AlignVCenter
            wrapMode: Text.Wrap
            rightPadding: 24
            clip: true
            font.pointSize: 26
            text: "<b>"+SpotifyLyricsApi.currentLineLyrics+"</b>"
        }
    }



    states: [
        State {
            name: "dataUnavailable"
            when: !SpotifyWebApi.dataAvailable || !SpotifyLyricsApi.available
            PropertyChanges { target: loader; sourceComponent: lyricsUnavailable }
        },

        State {
            name: "dataAvailable"
            when: SpotifyWebApi.dataAvailable && SpotifyLyricsApi.available
            PropertyChanges { target: loader; sourceComponent: lyricsAvailable }
        }
    ]
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.75;height:480;width:640}
}
##^##*/
