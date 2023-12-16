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

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QCursor>
#include <initializer_list>
#include <signal.h>
#include <unistd.h>
#include <QMetaType>

#include "bluezdevicemonitor.h"
#include "qdbusmetatype.h"

#include "app_environment.h"
#include "import_qml_plugins.h"

#include "utils.h"
#include "clockexposer.h"
#include "bluetoothmanager.h"
#include "spotifyauthmanager.h"
#include "qrimage.h"
#include "qrimageprovider.h"
#include "spotifywebapi.h"
#include "spotifylyricsapi.h"

#ifdef PI_ARM
#include <pigpio.h>
#include "pigpiobutton.h"
#else
#include "mockbutton.h"
#endif


#ifdef CAR_CRUISING_MOCK
    #include "speedmock.h"
    #include "rpmmock.h"
#else
    #include "speedexposer.h"
    #include "rpmexposer.h"
#endif

void catchUnixSignals(std::initializer_list<int> quitSignals) {
    auto handler = [](int sig) -> void {
        // blocking and not aysnc-signal-safe func are valid
        printf("\nquit the application by signal(%d).\n", sig);
        QCoreApplication::quit();
    };

    sigset_t blocking_mask;
    sigemptyset(&blocking_mask);
    for (auto sig : quitSignals)
        sigaddset(&blocking_mask, sig);

    struct sigaction sa;
    sa.sa_handler = handler;
    sa.sa_mask    = blocking_mask;
    sa.sa_flags   = 0;

    for (auto sig : quitSignals)
        sigaction(sig, &sa, nullptr);

#ifdef PI_ARM
    gpioTerminate();
#endif
}


int main(int argc, char *argv[])
{
    set_qt_environment();
    QGuiApplication app(argc, argv);
    QCoreApplication::setSetuidAllowed(true);

    catchUnixSignals({SIGQUIT, SIGINT, SIGTERM, SIGHUP, 11});

    auto mt = QMetaType::fromType<AccessToken>();
    qDebug() << mt.hasRegisteredDataStreamOperators();

    #ifdef QT_NO_DEBUG_OUTPUT
       qputenv("QT_LOGGING_RULES", "qml=false");
    #endif

    qRegisterMetaType<blueZ::InterfaceProperties>();
    qDBusRegisterMetaType<blueZ::InterfaceProperties>();
    qRegisterMetaType<blueZ::ObjectPathInterfaceMap>();
    qDBusRegisterMetaType<blueZ::ObjectPathInterfaceMap>();


    //rfkill
    blueZ::BlueZDeviceMonitor::unblockBluetooth();
    //alias
    blueZ::BlueZDeviceMonitor::setAdapterAlias("Dashboard");

    //weird hack, not really nice launching processes like this
    qDebug() << "Configuring DNS...";
    Utils::executeCommand("rm", {"/etc/resolv.conf"});
    std::vector<QString> resolvConfContent = {"nameserver 8.8.8.8", "nameserver 1.1.1.1"};
    Utils::writeFile("/etc/", "resolv.conf", resolvConfContent);    qDebug() << "DNS Configured";

    qmlRegisterSingletonType<ClockExposer>("ClockExposer",1,0,"ClockExposer", &ClockExposer::qmlInstance);
    qmlRegisterSingletonType<BluetoothManager>("BluetoothManager",1,0,"BluetoothManager", &BluetoothManager::qmlInstance);
    qmlRegisterSingletonType<SpotifyAuthManager>("SpotifyAuthManager", 1,0, "SpotifyAuthManager", &SpotifyAuthManager::qmlInstance);
    qmlRegisterSingletonType<QrImageProvider>("QrImageProvider",1,0,"QrImageProvider", &QrImageProvider::qmlInstance);
    qmlRegisterSingletonType<SpotifyWebApi>("SpotifyWebApi",1,0,"SpotifyWebApi", &SpotifyWebApi::qmlInstance);
    qmlRegisterSingletonType<SpotifyLyricsApi>("SpotifyLyricsApi",1,0,"SpotifyLyricsApi", &SpotifyLyricsApi::qmlInstance);

    qmlRegisterType<QrImage>("QrImage",1,0,"QrImage");

#ifdef CAR_CRUISING_MOCK
    qmlRegisterSingletonType<SpeedMock>("SpeedExposer",1,0,"SpeedExposer", &SpeedMock::qmlInstance);
    qmlRegisterSingletonType<RPMMock>("RPMExposer",1,0,"RPMExposer", &RPMMock::qmlInstance);
#else
    qmlRegisterSingletonType<SpeedExposer>("SpeedExposer",1,0,"SpeedExposer", &SpeedExposer::qmlInstance);
    qmlRegisterSingletonType<RPMExposer>("RPMExposer",1,0,"RPMExposer", &RPMExposer::qmlInstance);
#endif

#ifdef PI_ARM
    qmlRegisterType<PiGpioButton>("ButtonManager",1,0,"ButtonManager");
#else
    qmlRegisterType<MockButton>("ButtonManager",1,0,"ButtonManager");
#endif

    QCoreApplication::setOrganizationName("my");
    QCoreApplication::setOrganizationDomain("my");
    QCoreApplication::setApplicationName("Dashboard");

    QQmlApplicationEngine engine;
    const QUrl url(u"qrc:Main/main.qml"_qs);
    QObject::connect(
                &engine, &QQmlApplicationEngine::objectCreated, &app,
                [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    },
    Qt::QueuedConnection);

    engine.addImportPath(QCoreApplication::applicationDirPath() + "/qml");
    engine.addImportPath(":/");

    engine.load(url);

    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    QGuiApplication::setOverrideCursor(QCursor(Qt::BlankCursor));
    auto res = app.exec();

#ifdef PI_ARM
    gpioTerminate();
#endif

    return res;
}
