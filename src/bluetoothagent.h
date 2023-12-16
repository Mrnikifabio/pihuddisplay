#ifndef BLUETOOTHAGENT_H
#define BLUETOOTHAGENT_H

#include <QtDBus/QtDBus>

namespace blueZ
{

    class BluetoothAgent : public QObject
    {
        Q_OBJECT
        Q_CLASSINFO("D-Bus Interface", "org.bluez.Agent1")

    public:
        BluetoothAgent(QObject *parent = nullptr) : QObject(parent) {
            QDBusConnection dbus = QDBusConnection::systemBus();
            dbus.registerObject("/com/dashboardapp/agent", this, QDBusConnection::ExportAllSlots);
        }

    public slots:
        void Release() {
            // Implement cleanup here
        }

        void DisplayPinCode(const QDBusObjectPath &device, const QString &pincode) {
            qDebug() << "DisplayPinCode: Device" << device.path() << "Pincode:" << pincode;
            // Display the pincode to the user
        }

        void DisplayPasskey(const QDBusObjectPath &device, uint passkey) {
            qDebug() << "DisplayPasskey: Device" << device.path() << "Passkey:" << passkey;
            // Display the passkey to the user
        }

        void RequestConfirmation(const QDBusObjectPath &device, uint passkey) {
            qDebug() << "RequestConfirmation: Device" << device.path() << "Passkey:" << passkey;
            // Confirm the passkey displayed on the device
        }

        void Cancel() {
            qDebug() << "The pairing request was canceled";
        }
    };

}
#endif // BLUETOOTHAGENT_H
