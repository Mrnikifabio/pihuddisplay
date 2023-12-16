#ifndef BLUEZDEVICECONNECTION_H
#define BLUEZDEVICECONNECTION_H

#include <QObject>
#include <QDBusInterface>
#include <QDBusReply>
#include "bluezdevicemonitor.h"

namespace blueZ
{
    class BlueZDeviceConnection : public BlueZDeviceMonitor
    {
        Q_OBJECT

    public:
        BlueZDeviceConnection(const BlueZDevice& device, QObject *parent = nullptr);
        virtual ~BlueZDeviceConnection();

        void connectToNAP();
        void disconnectDevice();
        void unpairDevice();

    signals:
        void onDeviceConnected(const BlueZDevice &device);
        void onDeviceDisconnected(const BlueZDevice &device);

    private slots:
        void onDevicePropertiesChanged(const QString &interface,
                                       const QMap<QString, QVariant> &changedProperties,
                                       const QStringList &invalidatedProperties) override;

    private:
        const BlueZDevice currentDevice;
    };

}


#endif // BLUEZDEVICECONNECTION_H
