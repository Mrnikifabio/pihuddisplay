#ifndef BLUEZDEVICEPAIRER_H
#define BLUEZDEVICEPAIRER_H

#include "bluezdevicemonitor.h"
#include "bluetoothagent.h"
#include <QObject>

namespace blueZ {

    class BlueZDevicePairer : public BlueZDeviceMonitor
    {
        Q_OBJECT

    public:
        explicit BlueZDevicePairer(QObject *parent = nullptr);
        virtual ~BlueZDevicePairer();
        void beginPairing();
        void cancelPairing();
        void setAdapterDiscoverable(const bool discoverable = true);

    private:
        void registerBluetoothAgent();
        bool isPairing;
        BluetoothAgent *agent;
        BlueZDevice *pairedDevice;

    private slots:
        void onInterfaceAdded(const QDBusObjectPath &path, const QMap<QString, QMap<QString, QVariant>> &interfaces) override;
        void onDevicePropertiesChanged(const QString &interface,
                                       const QMap<QString, QVariant> &changedProperties,
                                       const QStringList &invalidatedProperties) override;

    signals:
        void onDevicePairedSuccess(const BlueZDevice &device);
    };
}

#endif // BLUEZDEVICEPAIRER_H
