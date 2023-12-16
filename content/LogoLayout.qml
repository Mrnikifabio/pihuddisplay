import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick3D 6.3
import Quick3DAssets.Volkslogo 1.0
import Dashboard


View3D {
    signal finalStateReached()
    environment: SceneEnvironment {
        antialiasingMode: SceneEnvironment.MSAA
        antialiasingQuality: SceneEnvironment.High
    }

    Node {

        DirectionalLight {
            id: directionalLight
            brightness: 1
        }

        PerspectiveCamera {
            id: camera
            x: 0
            y: -0
            z: 1172
        }

        Volkslogo {
        }

    }

    Item {
        id: __materialLibrary__
    }
    states: [
        State {
            name: "final state"

            PropertyChanges {
                target: camera
                z: 106
            }

            PropertyChanges {
                target: directionalLight
                brightness: 0
            }
        }
    ]



    transitions: [
        Transition{
            id: toFinalStateTransition
            SequentialAnimation {
                NumberAnimation {
                    target: camera
                    property: "z"
                    duration: 1600
                }
                NumberAnimation {
                    target: directionalLight
                    duration: 1300
                    property: "brightness"
                }

                ScriptAction {
                    script: {
                        finalStateReached()
                    }
                }
            }
            to: "final state"
        }
    ]

}



/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.9;height:480;width:640}D{i:4}
}
##^##*/
