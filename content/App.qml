/****************************************************************************
**
** Copyright (C) 2021 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Quick Studio Components.
**
** $QT_BEGIN_LICENSE:GPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 or (at your option) any later version
** approved by the KDE Free Qt Foundation. The licenses are as published by
** the Free Software Foundation and appearing in the file LICENSE.GPL3
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
** $QT_END_LICENSE$
**
****************************************************************************/

import QtQuick 6.2
import QtQuick.Window 6.2
import QtQuick.Controls 6.2
import Dashboard
import QtQuick3D
import Quick3DAssets.Vwgolf 1.0
import Quick3DAssets.Volkslogo 1.0
import BluetoothManager
import ButtonManager
import SpotifyAuthManager

Window {
    id: qtWindow
    property int rotationAngle: 90
    property bool targetIsEmbeded: BluetoothManager.embedded
    property string state: "empty state"
    width: Constants.width
    height: Constants.height

    visible: true
    maximumHeight: Constants.height
    maximumWidth: Constants.width
    minimumHeight: Constants.height
    minimumWidth: Constants.width
    title: "Dashboard"

    ButtonManager {
        id: rightButton
        gpioNumber: 22
        Component.onCompleted: {
            rightButton.installFilter(qtWindow)
        }
    }

    ButtonManager {
        id: middleButton
        gpioNumber: 17
        Component.onCompleted: {
            middleButton.installFilter(qtWindow)
        }
    }

    ButtonManager {
        id: leftButton
        gpioNumber: 27
        Component.onCompleted: {
            leftButton.installFilter(qtWindow)
        }
    }

    Connections {
        target: BluetoothManager
        onPairingFinishedSignal: {
            stack.pop()
            stack.push(mainScreen)
        }
        onPairingStartedSignal: {
            stack.pop()
            stack.push(pairingScreen)
        }
    }

    Connections {
        target: SpotifyAuthManager
        onAuthenticationStartedSignal: {
            stack.pop()
            stack.push(spotifyAuthScreen)
        }

        onAuthenticationFinishedSignal: {
            stack.pop()
            stack.push(mainScreen)
        }
    }

    Rectangle {
        gradient: Constants.backgroundGradient
        width: targetIsEmbeded ? qtWindow.height : qtWindow.width
        height: targetIsEmbeded ? qtWindow.width : qtWindow.height
        anchors.centerIn: parent
        rotation: targetIsEmbeded ? rotationAngle : 0
        Item {
            anchors.fill: parent
            // Rotate 90 degrees clockwise around transformOrigin
            transform:  Matrix4x4 { matrix: targetIsEmbeded ? Qt.matrix4x4( -1, 0, 0, Constants.width, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1) : Qt.matrix4x4() }
            StackView {
                id: stack
                initialItem: logoComponent
                anchors.fill: parent
                pushEnter: Constants.pushEnter
                popExit: Constants.popExit
                pushExit: Constants.pushExit
                popEnter: Constants.popEnter

            }
        }

    }


    Component {
        id: logoComponent
        LogoLayout {
            id: logoLayout
            onFinalStateReached: BluetoothManager.checkForSavedDeviceAndConnect()

            Timer {
                interval: 100
                running: true
                onTriggered: logoLayout.state="final state"
            }
        }
    }

    Component {
        id: pairingScreen
        PairingScreen {
            id: pairingScreenView
        }
    }

    Component {
        id: spotifyAuthScreen
        SpotifyAuth {
            id: spotifyAuthView
        }
    }

    Component {
        id: mainScreen

        Item {
            id: mainScreenView
            property int screenIndex: 0
            property var screens: [screen01, screen04, screen05, settings]
            property var carLayoutsStates: ["variation4Zoom", "variationFocus", "variationFocus", "variation5"]

            Connections {
                target: leftButton
                onButtonPressedSignal: {
                    //change current screen
                    screenIndex++
                    screenIndex = screenIndex === 4 ? 0 : screenIndex
                    screensStack.pop()
                    screensStack.push(screens[screenIndex])
                    carLayout.state = carLayoutsStates[screenIndex]
                }
            }

            CarLayout {
                id: carLayout
                anchors.fill: parent
                onFinalStateReached: screensStack.push(screen01)
            }

            StackView {
                id: screensStack
                anchors.fill: parent
                pushEnter: Constants.pushEnter
                popExit: Constants.popExit
                pushExit: Constants.pushExit
                popEnter: Constants.popEnter
            }

            Component {
                id: screen01
                Screen01 {
                    id: screen01View
                }
            }

            Component {
                id: screen04
                Screen04 {
                    id: screen04View
                }
            }

            Component {
                id: screen05
                Screen05 {
                    id: screen05View
                }
            }

            Component {
                id: settings
                Settings {
                    id: settingsView
                }
            }
        }
    }

    Item {
        id: __materialLibrary__
    }
}

