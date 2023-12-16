#include "serialdevice.h"

/*
 * The serial device aims to provide a relatively striaghtforward
 * interface to a serial device. You can find and connect to a device
 * . The only way to communicate with the device is by asking for
 * a write, and then any response will be returned through the use of
 * Qt's event system.
 *
 *
 * */
SerialDevice::SerialDevice()
{
    activePort = nullptr;
}
SerialDevice::~SerialDevice()
{
    Disconnect();
}

QSerialPort::SerialPortError SerialDevice::GetStatus()
{
    if (activePort)

        return activePort->error();
    else
        return QSerialPort::SerialPortError();
}

bool SerialDevice::FindDevices(int id)
{
    QList<QSerialPortInfo> portList = QSerialPortInfo::availablePorts();
    QSerialPortInfo port;

    if (activePort)
    {
        qWarning() << "Port already made with device. You should call disconnect() first";
    }

    foreach (port, portList)
    {
        if (port.productIdentifier() == id)
        {
            if (!port.isNull())
            {
                portInfo = port;
                activePort = new QSerialPort(port);
                return true;
            }
        }

    }

    // Go through all the matches and set the first non busy match
    // to our port info.


    qWarning() << "No possible devices found!";
    return false;
}


// Simply applies settings to the activeport.
void SerialDevice::ApplySettings(ConnectionInfo info)
{
    connectionInfo = info;
    if (activePort != nullptr)
    {
        activePort->setBaudRate(connectionInfo.baudRate);
        activePort->setDataBits(connectionInfo.databits);
        activePort->setStopBits(connectionInfo.stopbits);
        activePort->setFlowControl(connectionInfo.flowcontrol);
    }
    else
        qInfo() << "Port needs a device before you can apply settings!";
}

void SerialDevice::SetBaudRate(qint32 baud)
{
    if (activePort != nullptr)
    {
        activePort->setBaudRate(baud);
    }

    connectionInfo.baudRate = baud;
}

// Overload (use defaults)
void SerialDevice::ApplySettings()
{
    ApplySettings(connectionInfo);
}

// Tries to open a connection with the device.
// Returns false if fails.
bool SerialDevice::OpenConnection()
{
    if (!activePort)
    {
        qWarning() << "Port needs a device before opening.";
        return false;
    }

    if (activePort->isOpen())
    {
        qWarning() << "Called to open a connection, but connection is already open.";
        return false;
    }
    bool suc = activePort->open(QIODevice::ReadWrite);
    if (suc)
    {
        status = 3;
        connect(activePort,SIGNAL(errorOccurred(QSerialPort::SerialPortError))
                , this, SLOT(handleError(QSerialPort::SerialPortError)));
        return true;
    }
    else
        return false;
}

void SerialDevice::handleError(QSerialPort::SerialPortError error)
{
    status = 0;
    emit OnError(error);
    return;
}

// Close the connection, and remove any active
// signals/slots.
void SerialDevice::Disconnect()
{
    if (activePort != nullptr)
    {
        activePort->disconnect();
        if (activePort->isOpen())
        {
            activePort->clear();
            activePort->close();
            status = 0;
            delete(activePort);
            activePort = nullptr;

        }
    }
}

void SerialDevice::SendRecieve(QByteArray toWrite, bool override)
{
    QByteArray actual;
    if (!override &&!(status == 3))
    {
        qWarning() << "Asked to write when device is not ready.";
        return;
    }
    status = 2;

    readData.clear();
    readData.squeeze();
    msg.squeeze();

    if (toWrite.isEmpty())
    {
        // If toWrite is empty then just send a CR char.
        actual += ('\r');
    }
    else
    {
        // Check for CR at end.
        actual += toWrite;
        if (actual[actual.size()-1] != '\r')
            actual += '\r';

    }


    // Set status to writing

    // Connect events
    connect(activePort, SIGNAL(bytesWritten(qint64)), this, SLOT(HandleBytesWritten(qint64)));
    writeCount = actual.size();
    activePort->clear();
    activePort->write(actual);

    return;

}

void SerialDevice::HandleBytesWritten(qint64 bytesWritten)
{
    disconnect(activePort, SIGNAL(bytesWritten(qint64)), this, SLOT(HandleBytesWritten(qint64)));
    // Check for errors.
    // If eitheir of these cases happens the device has entered a dangerous sate.
    if (bytesWritten == -1)
    {
        qDebug() << QObject::tr("Failed to write data to port %1, error %2").arg(activePort->portName()).arg(activePort->errorString());
        status = 0;
        return;
    }
    else if (bytesWritten != writeCount)
    {
        qDebug() << QObject::tr("Failed to write all the data to port %1, error: %2").arg(activePort->portName()).arg(activePort->errorString());
        status = 0;
        return;
    }

    status = 1;

    connect(activePort, SIGNAL(readyRead()), SLOT(HandleReadyRead()));
}

void SerialDevice::HandleReadyRead()
{
    QByteArray raw;
    raw.append(activePort->readAll());

    foreach(char c, raw)
    {
        if (c == '>')
        {
            status = 3;
            disconnect(activePort, SIGNAL(readyRead()), this, SLOT(HandleReadyRead()));
            emit OnDataRecieve(readData);
            break;
        }
        else if (c == '\r')
        {
            if (msg.size() > 0)
            {
                emit OnMessage(msg);
                msg = QByteArray();
            }
        }
        else
        {
            msg.append(c);
            readData.append(c);
        }
    }

}

bool SerialDevice::Clear()
{
    if (activePort)
    {
        if (activePort->isOpen())
            if (activePort->clear())
                return true;
    }
    return false;
}

QByteArray SerialDevice::Interrupt()
{
    if (status != 3)
    {
        status = 3;
        disconnect(activePort, SIGNAL(readyRead()), this, 0);
        return readData;
    }
}
void SerialDevice::InterruptNoReturn()
{
    if (status != 3)
    {
        status = 3;
        disconnect(activePort, SIGNAL(readyRead()), this, 0);
    }
}
