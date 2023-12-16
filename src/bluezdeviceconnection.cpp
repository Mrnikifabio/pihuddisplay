#include "bluezdeviceconnection.h"

using namespace blueZ;

static constexpr char napUUID[] = "00001116-0000-1000-8000-00805f9b34fb";  // UUID for NAP service

void BlueZDeviceConnection::connectToNAP()
{
    QDBusInterface networkInterface("org.bluez", currentDevice.getPath(), "org.bluez.Network1", QDBusConnection::systemBus());
    QDBusReply<QString> reply = networkInterface.call("Connect", napUUID);

    if (!reply.isValid()) {
        // Handle error
        qDebug() << "Failed to connect to NAP:" << reply.error().message();
    } else {
        QString interfaceName = reply.value();
        qDebug() << "NAP Connection established " << interfaceName;
    }
}

void BlueZDeviceConnection::disconnectDevice()
{
    QDBusInterface deviceInterface("org.bluez", currentDevice.getPath(), "org.bluez.Device1", QDBusConnection::systemBus());
    deviceInterface.call("Disconnect");
}

void BlueZDeviceConnection::unpairDevice()
{
    QDBusInterface adapterInterface("org.bluez",
                                    "/org/bluez/hci0",
                                    "org.bluez.Adapter1",
                                    QDBusConnection::systemBus());

    if (adapterInterface.isValid()) {
        QDBusMessage response = adapterInterface.call("RemoveDevice", QVariant::fromValue(QDBusObjectPath(currentDevice.getPath())));

        if (response.type() == QDBusMessage::ErrorMessage) {
            qDebug() << "DBus Error:" << response.errorName() << response.errorMessage();
            // Handle error accordingly
        } else {
            qDebug() << "Device unpaired/deleted successfully.";
        }
    } else {
        qDebug() << "Failed to connect to DBus interface.";
        // Handle error accordingly
    }
}

blueZ::BlueZDeviceConnection::BlueZDeviceConnection(const BlueZDevice &device, QObject *parent) : BlueZDeviceMonitor(parent), currentDevice{std::move(device)}
{
    monitorDevice(device.getPath());
}

blueZ::BlueZDeviceConnection::~BlueZDeviceConnection()
{
    disconnectDevice();
}

void blueZ::BlueZDeviceConnection::onDevicePropertiesChanged(const QString &interface, const QMap<QString, QVariant> &changedProperties, const QStringList &invalidatedProperties)
{
    if (interface == "org.bluez.Device1" && changedProperties.contains("Connected")) {
        bool isConnected = changedProperties.value("Connected").toBool();
        qDebug() << "Device" << (isConnected ? "connected:" : "disconnected:") << interface;

        if(isConnected)
            emit onDeviceConnected(currentDevice);
        else
            emit onDeviceDisconnected(currentDevice);
    }
}


