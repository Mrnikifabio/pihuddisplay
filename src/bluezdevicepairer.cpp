#include "bluezdevicepairer.h"

#include <QDBusInterface>
#include <QDBusReply>

using namespace blueZ;

BlueZDevicePairer::BlueZDevicePairer(QObject *parent)
    : BlueZDeviceMonitor{parent}, isPairing{false}, agent{nullptr}, pairedDevice{nullptr}
{}

void BlueZDevicePairer::setAdapterDiscoverable(const bool discoverable)
{
    QDBusInterface adapterInterface{"org.bluez", "/org/bluez/hci0", "org.freedesktop.DBus.Properties", QDBusConnection::systemBus()};
    QDBusReply<void> reply = adapterInterface.call("Set", "org.bluez.Adapter1", "Discoverable", QVariant::fromValue(QDBusVariant(discoverable)));
    if (!reply.isValid()) {
        qDebug() << "Error setting discoverable:" << reply.error().message();
    }
}

void BlueZDevicePairer::beginPairing()
{
    registerBluetoothAgent();
    setAdapterDiscoverable();
    monitorInterface();
}

void BlueZDevicePairer::cancelPairing()
{
    if(isPairing)
    {
        qDebug() << "Cancel pairing";
        agent->Cancel();
        if(pairedDevice != nullptr)
        {
            delete pairedDevice;
            pairedDevice = nullptr;
        }
    }
}

void BlueZDevicePairer::registerBluetoothAgent()
{
    if(agent != nullptr)
    {
        qDebug() << "Agent already registered";
        return;
    }

    agent = new BluetoothAgent();
    QDBusInterface agentManager("org.bluez", "/org/bluez", "org.bluez.AgentManager1", QDBusConnection::systemBus());
    QDBusMessage reply = agentManager.call("RegisterAgent", QVariant::fromValue(QDBusObjectPath("/com/dashboardapp/agent")), "DisplayOnly");

    if (reply.type() == QDBusMessage::ErrorMessage) {
        qDebug() << "Failed to register agent:" << reply.errorMessage();
    } else {
        qDebug() << "Agent registered successfully";
        agentManager.call("RequestDefaultAgent", QVariant::fromValue(QDBusObjectPath("/com/dashboardapp/agent")));
    }
}

void BlueZDevicePairer::onInterfaceAdded(const QDBusObjectPath &path, const QMap<QString, QMap<QString, QVariant>> &interfaces)
{
    if (interfaces.contains("org.bluez.Device1")) {
        isPairing = true;
        // A new device has been detected, monitor its properties
        setAdapterDiscoverable(false);
        pairedDevice = new BlueZDevice(path.path(), interfaces.value("org.bluez.Device1").value("Alias").toString());
        monitorDevice(pairedDevice->getPath());
    }
}

void BlueZDevicePairer::onDevicePropertiesChanged(const QString &interface, const QMap<QString, QVariant> &changedProperties, const QStringList &invalidatedProperties)
{
    if (interface == "org.bluez.Device1" && changedProperties.contains("Paired")) {
        bool isPaired = changedProperties.value("Paired").toBool();
        if (isPaired)
        {
            isPairing = false;
            emit onDevicePairedSuccess(*pairedDevice);
        }
    }
}

BlueZDevicePairer::~BlueZDevicePairer()
{
    if(pairedDevice != nullptr)
        delete pairedDevice;

    if(agent != nullptr)
    {
        QDBusInterface agentManager("org.bluez", "/org/bluez", "org.bluez.AgentManager1", QDBusConnection::systemBus());
        auto d = agentManager.call("UnregisterAgent", QVariant::fromValue(QDBusObjectPath("/com/dashboardapp/agent")));
        if(d.type() == QDBusMessage::ErrorMessage)
        {
            qDebug() << "Error while unregistering agent "<< d.errorMessage();
        }

        delete agent;
    }
}
