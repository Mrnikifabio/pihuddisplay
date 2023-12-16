#ifndef BLUEZDEVICEMONITOR_H
#define BLUEZDEVICEMONITOR_H

#include <QDBusObjectPath>
#include <QObject>

namespace blueZ {

    class BlueZDevice {
        QString path;
        QString alias;

    public:
        // Constructor
        BlueZDevice(const QString& path, const QString& alias)
            : path(path), alias(alias) {}

        const QString& getPath() const { return path; }
        const QString& getAlias() const { return alias; }
    };

    // Define the inner map (interface properties)
    typedef QMap<QString, QVariantMap> InterfaceProperties;

    // Define the outer map (object path to interface properties)
    typedef QMap<QDBusObjectPath, InterfaceProperties> ObjectPathInterfaceMap;

    class BlueZDeviceMonitor : public QObject
    {
        Q_OBJECT
    public:
        explicit BlueZDeviceMonitor(QObject *parent = nullptr);
        virtual ~BlueZDeviceMonitor();
        static QVector<BlueZDevice> getKnownDevices();
        static bool unblockBluetooth();
        static void setAdapterAlias(const QString &name);

    private slots:
        virtual void onInterfaceAdded(const QDBusObjectPath &path, const QMap<QString, QMap<QString, QVariant>> &interfaces);
        virtual void onDevicePropertiesChanged(const QString &interface, const QMap<QString, QVariant> &changedProperties,
                                               const QStringList &invalidatedProperties) = 0;
    private:
        QString devicePath;

    protected:
        void monitorInterface();
        void monitorDevice(const QString &devicePath);
    };

}

#endif // BLUEZDEVICEMONITOR_H
