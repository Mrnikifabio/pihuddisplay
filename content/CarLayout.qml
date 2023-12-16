import QtQuick 2.15
import QtQuick.Controls 2.15
import Quick3DAssets.Vwgolf 1.0
import QtQuick3D 6.3
import QtQuick3D.Effects

View3D {
    id: root
    signal finalStateReached()
    property int pauseDuration: 350
    property int animDuration: 600

    Timer {
        interval: 250
        running: true
        onTriggered: root.state = "variation1"
    }

    environment: sceneEnvironment
    SceneEnvironment {
        id: sceneEnvironment
        antialiasingMode: SceneEnvironment.MSAA
        antialiasingQuality: SceneEnvironment.High

    }

    Node {
        id: scene

        PerspectiveCamera {
            id: sceneCamera
            x: -0
            y: 0
            z: 398.78537
            fieldOfView: 45
            fieldOfViewOrientation: PerspectiveCamera.Horizontal
            clipFar: 324000
        }

        Vwgolf {
            id: vW_Golf6_5doors
        }

        DirectionalLight {
            id: lightDirectional
        }
    }

    Item {
        id: __materialLibrary__
    }

    states: [
        State {
            name: "variation1"

            PropertyChanges {
                target: sceneCamera
                x: -337.122
                y: -156.871
                eulerRotation.z: -80.99571
                eulerRotation.y: -72.67142
                eulerRotation.x: 26.86986
                z: 128.02689
            }
        },
        State {
            name: "variation2"
            PropertyChanges {
                target: sceneCamera
                x: -336.844
                y: 156.972
                eulerRotation.x: -26.86986
                z: 127.94022
                eulerRotation.y: -72.67144
                eulerRotation.z: -94.73558
            }
        },
        State {
            name: "variation3"
            PropertyChanges {
                target: sceneCamera
                x: -1.952
                y: 348.971
                eulerRotation.x: -70.672
                z: 153.08052
                eulerRotation.y: 1.42508
                eulerRotation.z: 179.55936
            }
        },
        State {
            name: "variation4"
            PropertyChanges {
                target: sceneCamera
                x: 867.254
                y: 0.026
                eulerRotation.x: -0.0441
                z: 102.77485
                eulerRotation.y: 90.18118
                eulerRotation.z: 90.77145
            }
        },
        State {
            name: "variation4Zoom"
            PropertyChanges {
                target: sceneCamera
                x: 617.218
                y: 0.053
                eulerRotation.x: -0.00298
                z: 87.24483
                eulerRotation.y: 87.13071
                eulerRotation.z: 90.77271
            }

        },
        State {
            name: "variationFocus"
            PropertyChanges {
                target: lightDirectional
                brightness: 0
            }
            PropertyChanges {
                target: sceneCamera
                x: 617.218
                y: 0.053
                eulerRotation.x: -0.00298
                z: 87.24483
                eulerRotation.y: 87.13071
                eulerRotation.z: 90.77271
            }
        },
        State {
            name: "variation5"
            PropertyChanges {
                target: sceneCamera
                x: -407.531
                y: -110.319
                eulerRotation.z: -83.53615
                eulerRotation.y: -78.37801
                eulerRotation.x: 29.6215
                z: 106.08674
            }
        }
    ]

    transitions: [
        Transition {
            id: fromDefto1
            SequentialAnimation {
                PauseAnimation {
                    duration: root.pauseDuration
                }
                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }

                ScriptAction {
                    script: {
                        root.state = "variation2"
                    }
                }
            }
            from: ""
            to:"variation1"
        },
        Transition {
            id: from1to2
            SequentialAnimation {
                PauseAnimation {
                    duration: root.pauseDuration
                }

                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }

                ScriptAction {
                    script: {
                        root.state = "variation3"
                    }
                }
            }
            from:"variation1"
            to:"variation2"
        },
        Transition {
            id: from2to3
            SequentialAnimation {
                PauseAnimation {
                    duration: root.pauseDuration
                }

                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }

                ScriptAction {
                    script: {
                        root.state = "variation4Zoom"
                    }
                }
            }
            from:"variation2"
            to:"variation3"
        },
        Transition {
            id: from3to4
            SequentialAnimation {
                PauseAnimation {
                    duration: root.pauseDuration
                }

                ParallelAnimation {

                    NumberAnimation {
                        target: lightDirectional
                        properties:"brightness"
                        duration: pauseDuration
                    }

                    NumberAnimation {
                        target: sceneCamera
                        properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                        duration: animDuration
                    }
                }

                ScriptAction {
                    script: {
                        finalStateReached()
                    }
                }
            }
            from:"variation3"
            to:"variation4Zoom"
        },
        Transition {
            id: from4toZoom
            SequentialAnimation {
                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }
            }
            from:"variation4Zoom"
            to:"variation4"
        },
        Transition {
            id: fromZoomtoFocus
            SequentialAnimation {
                NumberAnimation {
                    target: lightDirectional
                    properties: "brightness"
                    duration: pauseDuration
                }

                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }
            }
            from:"variation4Zoom"
            to:"variationFocus"
        },
        Transition {
            id: fromFocusto5
            ParallelAnimation {
                NumberAnimation {
                    target: lightDirectional
                    properties:"brightness"
                    duration: pauseDuration
                }
                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }
            }
            from:"variationFocus"
            to:"variation5"
        },
        Transition {
            id: fromFocusto4Zoom
            SequentialAnimation {
                NumberAnimation {
                    target: lightDirectional
                    properties:"brightness"
                    duration: pauseDuration
                }
                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }
            }
            from:"variationFocus"
            to:"variation4Zoom"
        },
        Transition {
            id: from5to4Zoom
            SequentialAnimation {
                NumberAnimation {
                    target: lightDirectional
                    properties:"brightness"
                    duration: pauseDuration
                }
                NumberAnimation {
                    target: sceneCamera
                    properties: "x,y,z,eulerRotation.x,eulerRotation.y,eulerRotation.z"
                    duration: animDuration
                }
            }
            from:"variation5"
            to:"variation4Zoom"
        }
    ]
}



/*##^##
Designer {
    D{i:0;autoSize:true;formeditorColor:"#000000";formeditorZoom:0.5;height:400;width:1280}
}
##^##*/
