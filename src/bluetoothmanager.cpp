#include "bluetoothmanager.h"
#include "bluezdevicemonitor.h"
#include "utils.h"
#include <QDebug>
#include <QThreadPool>
#include <QtQml/qqml.h>

#define TIMEOUT 25000

QObject* BluetoothManager::m_instance{nullptr};

BluetoothManager::BluetoothManager(QObject *parent)
    : m_connected{false}, m_paired{false}, m_pairedDeviceName{""},
    m_pairingTimer{new QTimer{this}}, m_connection{nullptr}, m_pairer{nullptr}, m_noConnectionTimer{new QTimer{this}}, QObject{parent}
{
    connect(m_pairingTimer, &QTimer::timeout, this, &BluetoothManager::pairingTimeout);
    connect(m_noConnectionTimer, &QTimer::timeout, this, &BluetoothManager::noConnectionTimeout);
    m_noConnectionTimer->start(10000);
}


QObject *BluetoothManager::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new BluetoothManager();

    return m_instance;
}

void BluetoothManager::checkForSavedDeviceAndConnect()
{
    auto devices = blueZ::BlueZDeviceMonitor::getKnownDevices();
    if(devices.size() > 0)
    {
        if(m_connection == nullptr)
        {
            auto device = devices.first();
            setPairedDeviceName(device.getAlias());
            setIsPaired(true);
            m_connection = new blueZ::BlueZDeviceConnection(device);
            connect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceConnected, this, &BluetoothManager::connected);
            connect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceDisconnected, this, &BluetoothManager::disconnected);
            m_connection->connectToNAP();
            emit pairingFinishedSignal();
        }
        else
        {
            qDebug() << "Connection already present, trying to connect";
            m_connection->connectToNAP();
        }
    }
    else
    {
        setIsConnected(false);
        emit pairingFinishedSignal();
    }
}

void BluetoothManager::setDiscoverableAndBeginPairing()
{
    emit pairingStartedSignal();
    if(m_pairer != nullptr)
    {
        disconnect(m_pairer, &blueZ::BlueZDevicePairer::onDevicePairedSuccess, this, &BluetoothManager::paired);
        delete m_pairer;
        m_pairer = nullptr;
    }

    if(m_connection != nullptr)
    {
        m_connection->unpairDevice();
        setIsConnected(false);
        disconnect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceConnected, this, &BluetoothManager::connected);
        disconnect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceDisconnected, this, &BluetoothManager::disconnected);
        delete m_connection;
        m_connection = nullptr;
    }

    m_pairingTimer->setSingleShot(true);
    m_pairingTimer->start(TIMEOUT);
    m_pairer = new blueZ::BlueZDevicePairer();
    connect(m_pairer, &blueZ::BlueZDevicePairer::onDevicePairedSuccess, this, &BluetoothManager::paired);
    setIsPaired(false);
    m_pairer->beginPairing();
}

void BluetoothManager::cancelPairing()
{
    //we can cancel the pairing procedure only if we are waiting for a device to be connected to the device
    if(m_pairingTimer->isActive())
    {
        m_pairer->cancelPairing();
        m_pairer->setAdapterDiscoverable(false);
        m_pairingTimer->stop();
        setIsPaired(false);
        setIsConnected(false);
        emit pairingFinishedSignal();
    }
}

void BluetoothManager::connected(const blueZ::BlueZDevice &dev)
{
    setIsConnected(true);
    Utils::renewDhcpLease();
}

void BluetoothManager::disconnected(const blueZ::BlueZDevice &dev)
{
    setIsConnected(false);
}

void BluetoothManager::paired(const blueZ::BlueZDevice &device)
{
    m_pairingTimer->stop();

    if(m_connection != nullptr)
        delete m_connection;

    setPairedDeviceName(device.getAlias());
    emit connectingToNAPAfterPairingSignal();
    m_connection = new blueZ::BlueZDeviceConnection(device);
    connect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceConnected, this, &BluetoothManager::connected);
    connect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceDisconnected, this, &BluetoothManager::disconnected);
    m_connection->connectToNAP();
    setIsConnected(true);
    Utils::renewDhcpLease();
    setIsPaired(true);
    emit pairingFinishedSignal();
}

void BluetoothManager::pairingTimeout()
{
    qDebug() << "Pairing failed due to timout";
    m_pairer->cancelPairing();
    m_pairer->setAdapterDiscoverable(false);
    setIsPaired(false);
    setIsConnected(false);
    m_pairingTimer->stop();
    emit pairingFinishedSignal();
}

void BluetoothManager::noConnectionTimeout()
{
    if(isPaired() && !isConnected())
    {
        qDebug() << "No connection timeout, trying to connect";
        QThreadPool::globalInstance()->start([this]{
            m_connection->connectToNAP();
        });
    }
}

QString BluetoothManager::getPairedDeviceName() const
{
    return m_pairedDeviceName;
}

bool BluetoothManager::isConnected() const
{
    return m_connected;
}

bool BluetoothManager::isPaired() const
{
    return m_paired;
}

bool BluetoothManager::isTargetEmbedded() const
{
#ifdef PI_ARM
    return true;
#else
    return false;
#endif
}

void BluetoothManager::setIsConnected(bool isConnected)
{
    if(m_connected != isConnected)
    {
        m_connected = isConnected;
        emit isConnectedChangedSignal();
    }
}

void BluetoothManager::setIsPaired(bool isPaired)
{
    if(m_paired != isPaired)
    {
        m_paired = isPaired;
        emit isPairedChangedSignal();
    }
}

void BluetoothManager::setPairedDeviceName(QString name)
{
    if(name != m_pairedDeviceName)
    {
        m_pairedDeviceName = name;
        emit pairedDeviceNameChangedSignal();
    }
}

BluetoothManager::~BluetoothManager()
{
    delete m_pairingTimer;
    m_noConnectionTimer->stop();
    delete m_noConnectionTimer;
    if(m_pairer != nullptr)
    {
        disconnect(m_pairer, &blueZ::BlueZDevicePairer::onDevicePairedSuccess, this, &BluetoothManager::paired);
        delete m_pairer;
    }

    if(m_connection != nullptr)
    {
        if(isConnected())
            m_connection->disconnectDevice();

        disconnect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceConnected, this, &BluetoothManager::connected);
        disconnect(m_connection, &blueZ::BlueZDeviceConnection::onDeviceDisconnected, this, &BluetoothManager::disconnected);
        delete m_connection;
    }
}
