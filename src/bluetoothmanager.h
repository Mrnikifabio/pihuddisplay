#ifndef BLUETOOTHMANAGER_H
#define BLUETOOTHMANAGER_H

#include <QObject>
#include <QQmlEngine>
#include <QtConcurrent/QtConcurrent>
#include "bluezdeviceconnection.h"
#include "bluezdevicepairer.h"


class BluetoothManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString pairedDeviceName READ getPairedDeviceName NOTIFY pairedDeviceNameChangedSignal)
    Q_PROPERTY(bool connected READ isConnected NOTIFY isConnectedChangedSignal)
    Q_PROPERTY(bool paired READ isPaired NOTIFY isPairedChangedSignal)
    Q_PROPERTY(bool embedded READ isTargetEmbedded CONSTANT)
    Q_DISABLE_COPY(BluetoothManager)
    BluetoothManager(QObject *parent = nullptr);
    ~BluetoothManager();

private:
    static QObject *m_instance;
    bool m_connected;
    bool m_paired;
    QString m_pairedDeviceName;
    QTimer *m_pairingTimer;
    QTimer *m_noConnectionTimer;
    blueZ::BlueZDeviceConnection *m_connection;
    blueZ::BlueZDevicePairer *m_pairer;

public:
    static QObject *qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);
    bool isConnected() const;
    bool isPaired() const;
    bool isTargetEmbedded() const;
    QString getPairedDeviceName() const;
    Q_INVOKABLE void checkForSavedDeviceAndConnect();
    Q_INVOKABLE void setDiscoverableAndBeginPairing();

public slots:
    void setIsConnected(bool isConnected);
    void setIsPaired(bool isPaired);
    void setPairedDeviceName(QString name);
    void cancelPairing();
    void connected(const blueZ::BlueZDevice &dev);
    void disconnected(const blueZ::BlueZDevice &dev);
    void paired(const blueZ::BlueZDevice &device);
    void pairingTimeout();
    void noConnectionTimeout();

signals:
    void pairingFinishedSignal();
    void pairingStartedSignal();
    void connectingToNAPAfterPairingSignal();

    void isConnectedChangedSignal();
    void isPairedChangedSignal();
    void pairedDeviceNameChangedSignal();
};

#endif // BLUETOOTHMANAGER_H
