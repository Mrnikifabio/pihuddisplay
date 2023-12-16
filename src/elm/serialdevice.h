#ifndef SERIALDEVICE_H
#define SERIALDEVICE_H

#include <QtSerialPort/QtSerialPort>

struct ConnectionInfo
{
    qint32 baudRate = 38400;
    QSerialPort::DataBits databits = QSerialPort::Data8;
    QSerialPort::StopBits stopbits = QSerialPort::OneStop;
    QSerialPort::FlowControl flowcontrol = QSerialPort::NoFlowControl;

};

class SerialDevice : public QObject
{
    Q_OBJECT

private:
    QSerialPortInfo portInfo;
    ConnectionInfo connectionInfo;
    QSerialPort *activePort;
    /* 0 = not connected or error;
     * 1 = waiting on read
     * 2 = waiting on write
     * 3 = ready
     * */

    qint64 writeCount;
    QByteArray readData;
    QByteArray msg;



public:
    SerialDevice();
    qint32 status = 0;
    QSerialPort::SerialPortError GetStatus();
    bool FindDevices(int id);
    void ApplySettings(ConnectionInfo info);
    void ApplySettings();


    // Connection
    // Default suits default elm327


    void Disconnect();
    bool OpenConnection();
    void SendRecieve(QByteArray toWrite, bool override = false);
    ~SerialDevice();
    bool Clear();
    void SetBaudRate(qint32 baud);
    QByteArray Interrupt();
    void InterruptNoReturn();
signals:
    void OnDataRecieve(QByteArray);
    void OnError(QSerialPort::SerialPortError);
    void OnMessage(QByteArray);
private slots:
    void handleError(QSerialPort::SerialPortError error);
    void HandleBytesWritten(qint64 bytesWritten);
    void HandleReadyRead();
};

#endif // SERIALDEVICE_H
