#include "bluezdevicemonitor.h"

#include <QDBusInterface>
#include <qdbusreply.h>

#include <QFile>
#include <QIODevice>
#include <QDebug>
#include <cstring>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <linux/rfkill.h>
#include <cerrno>

using namespace blueZ;

BlueZDeviceMonitor::BlueZDeviceMonitor(QObject *parent)
    : QObject{parent}
{}

BlueZDeviceMonitor::~BlueZDeviceMonitor()
{
    QDBusConnection::systemBus().disconnect(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        "InterfacesAdded",
        this,
        SLOT(onInterfaceAdded(QDBusObjectPath,QMap<QString,QMap<QString,QVariant>>))
        );

    QDBusConnection::systemBus().disconnect(
        "org.bluez",
        devicePath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        this,
        SLOT(onDevicePropertiesChanged(QString,QMap<QString,QVariant>,QStringList))
        );
}

const QDBusArgument &operator>>(const QDBusArgument &argument, InterfaceProperties &properties) {
    argument.beginMap();
    while (!argument.atEnd()) {
        QString key;
        QVariantMap value;
        argument.beginMapEntry();
        argument >> key >> value;
        properties.insert(key, value);
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

// Overloading for ObjectPathInterfaceMap
QDBusArgument &operator<<(QDBusArgument &argument, const ObjectPathInterfaceMap &map) {
    argument.beginMap(QMetaType::QString, QMetaType::QVariantMap);
    for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
        argument.beginMapEntry();
        argument << it.key() << it.value();
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

const QDBusArgument &operator>>(const QDBusArgument &argument, ObjectPathInterfaceMap &map) {
    argument.beginMap();
    while (!argument.atEnd()) {
        QDBusObjectPath key;
        InterfaceProperties value;
        argument.beginMapEntry();
        argument >> key >> value;
        map.insert(key, value);
        argument.endMapEntry();
    }
    argument.endMap();
    return argument;
}

QVector<BlueZDevice> BlueZDeviceMonitor::getKnownDevices()
{
    QVector<BlueZDevice>  va;

    QDBusInterface objectManager("org.bluez", "/", "org.freedesktop.DBus.ObjectManager", QDBusConnection::systemBus());
    QDBusReply<ObjectPathInterfaceMap> reply = objectManager.call("GetManagedObjects");

    if (reply.isValid()) {
        ObjectPathInterfaceMap map = reply.value();

        for (auto it = map.constBegin(); it != map.constEnd(); ++it) {
            const QDBusObjectPath &objectPath = it.key();
            const InterfaceProperties &interfaces = it.value();


            // Check for the Bluetooth device interface
            if (interfaces.contains("org.bluez.Device1")) {
                const QVariantMap &deviceProperties = interfaces.value("org.bluez.Device1");

                // Check if the device is paired
                if (deviceProperties.contains("Alias") && deviceProperties.contains("Paired") && deviceProperties.value("Paired").toBool()) {
                    qDebug() << "Paired Bluetooth device found at:" << objectPath.path();
                    // Process the paired device as needed
                    va.push_back({objectPath.path(), deviceProperties.value("Alias").toString()});
                }
            }
        }

    } else {
        qDebug() << "D-Bus call failed:" << reply.error().message();
    }

    return va;
}

void BlueZDeviceMonitor::onInterfaceAdded(const QDBusObjectPath &path, const QMap<QString, QMap<QString, QVariant>> &interfaces)
{
    qDebug() << "onInterfaceAdded";
}

void BlueZDeviceMonitor::monitorInterface()
{
    QDBusConnection::systemBus().connect(
        "org.bluez",
        "/",
        "org.freedesktop.DBus.ObjectManager",
        "InterfacesAdded",
        this,
        SLOT(onInterfaceAdded(QDBusObjectPath,QMap<QString,QMap<QString,QVariant>>))
        );
}


void BlueZDeviceMonitor::monitorDevice(const QString &devicePath)
{
    QDBusConnection::systemBus().connect(
        "org.bluez",
        devicePath,
        "org.freedesktop.DBus.Properties",
        "PropertiesChanged",
        this,
        SLOT(onDevicePropertiesChanged(QString,QMap<QString,QVariant>,QStringList))
        );

    this->devicePath = devicePath;
}

bool BlueZDeviceMonitor::unblockBluetooth() {
    qDebug() << "Trying to unblock bluetooth";
    int idx = 0;
    int fd = open("/dev/rfkill", O_RDWR);
    if (fd < 0) {
        qDebug() << "Error opening /dev/rfkill";
        return false;
    }

    struct rfkill_event id_event;
    while (read(fd, &id_event, sizeof(id_event)) > 0) {
        if (id_event.type == RFKILL_TYPE_BLUETOOTH) {
            idx = id_event.idx;
            qDebug() << "Found idx";
            break;
        }
    }

    qDebug() << "idx is" << idx;

    struct rfkill_event event;
    memset(&event, 0, sizeof(event));
    event.idx = idx;  // Index of the Bluetooth device
    event.type = RFKILL_TYPE_BLUETOOTH;
    event.op = RFKILL_OP_CHANGE;
    event.soft = 0;  // Soft unblock

    ssize_t len = write(fd, &event, sizeof(event));
    if (len < 0) {
        int err = errno;
        qDebug() << "Error writing to /dev/rfkill" << strerror(err);
        return false;
    }

    close(fd);
    return true;
}

void BlueZDeviceMonitor::setAdapterAlias(const QString &name)
{
    QDBusInterface adapterInterface("org.bluez",
                                    "/org/bluez/hci0",
                                    "org.freedesktop.DBus.Properties",
                                    QDBusConnection::systemBus());

    if (adapterInterface.isValid()) {
        QDBusMessage response = adapterInterface.call("Set", "org.bluez.Adapter1", "Alias", QVariant::fromValue(QDBusVariant(name)));
        if(response.type() == QDBusMessage::ErrorMessage)
            qDebug() << "Invalid interface, cannot change alias";
        else
            qDebug() << "New alias " << name << " set";
    } else {
        qDebug() << "Invalid interface, cannot change alias";
    }
}


Q_DECLARE_METATYPE(blueZ::InterfaceProperties)
Q_DECLARE_METATYPE(blueZ::ObjectPathInterfaceMap)

