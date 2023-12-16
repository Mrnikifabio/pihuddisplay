#include "elm.h"
#include <QtConcurrent/QtConcurrent>
#include <memory>
/*
 * The elm class mostly serves to serve as a higher level method of accessing the serialdevice
 *
 *
 * */
Elm::Elm()
{
    doingJobs = false;
}
Elm::~Elm()
{

}

void Elm::ConnectToDevice(qint32 productID)
{
    if (!FindDevices(productID))
        {
            emit OnDeviceFail(DeviceError::NoDevicesFound);
        }
        else
        {
            ApplySettings();
            bool suc = OpenConnection();

            if (suc)
            {
                // Clear buffer of anything that may interfere
                if (Clear())
                {


                    emit OnDeviceReady();

                        auto conn = std::make_shared<QMetaObject::Connection>();
                        // Setup timer, mostly for identifying a bad baud rate
                        simpleTimer.setInterval(200);
                        simpleTimer.setSingleShot(true);

                        connect(&simpleTimer, &QTimer::timeout, this, [this](){
                           emit SerialDevice::OnDataRecieve(Interrupt());
                        });

                        // Reset Device
                        connect(this, &SerialDevice::OnError, this, &Elm::OnSerialPortError);
                        *conn = connect(this, &SerialDevice::OnDataRecieve, [this, conn](QByteArray a) {
                            disconnect(*conn);
                            if (a.contains("ELM"))
                            {
                                simpleTimer.stop();
                                *conn = connect(this, &SerialDevice::OnDataRecieve, [this, conn](QByteArray a) {
                                    disconnect(*conn);
                                    if (!a.contains("OK"))
                                        GeneralDeviceError(DeviceError::UnknownError);
                                });
                                SendRecieve("ATE0");
                            }
                            else
                            {
                                GeneralDeviceError(DeviceError::BadBaudRate);
                            }
                        });


                        // Resets the device.
                        // Once the program is more stable this won't be needed.
                        // Afterwards InitailiseDevice can be called directly.

                        ResetDevice();
                        simpleTimer.start();

                }
                else
                {
                    qWarning() << "Couldn't clear buffer.";
                    status = 0;
                    emit OnDeviceFail(DeviceError::UnknownError);

                }

            }
            else
            {
                qWarning() << "Couldn't open port";
                status = 0;
                emit OnDeviceFail(DeviceError::NotOpenError);
            }
        }
}

void Elm::OnSerialPortError(QSerialPort::SerialPortError error)
{
    GeneralDeviceError((DeviceError)error);
}

void Elm::GeneralDeviceError(DeviceError error)
{
    qInfo() << error;
    Disconnect();
    emit OnDeviceFail((DeviceError)error);
}

void Elm::ResetDevice()
{
    SendRecieve("ATWS");
}

void Elm::ChangeProtocol()
{

}

void Elm::AddJob(QByteArray toSend)
{
    AddJob(toSend, NULL, NULL);
}

void Elm::AddJob(QByteArray toSend, std::function<void(Job)> func,
                 std::function<void(QByteArray)> funcAfterMsg)
{
    if (status == 0)
    {
        qWarning() << "Not ready to recieve jobs!";
    }
    else

    {
        command newCommand;
        newCommand.funcToCall = func;
        newCommand.cmd = toSend;
        newCommand.funcToCallAfterMessage = funcAfterMsg;
        commands.enqueue(newCommand);

        if (!doingJobs)
        {
            doingJobs = true;
            DoNextJob();
        }
    }

}

void Elm::OnJobFinish(QByteArray arr)
{
    disconnect(this,SIGNAL(OnDataRecieve(QByteArray)), this, SLOT(OnJobFinish(QByteArray)));
    disconnect(this, &SerialDevice::OnMessage, this, &Elm::OnMessageRecieved);
    currentJob.execution_time = QDateTime::currentMSecsSinceEpoch() - timeStart;
    currentJob.response = ParseResponse(arr);
    std::function<void(Job)> toCall = currentJobCommand.funcToCall;

    // Call function for finished job if exists

    emit OnJobFinished(currentJob);
    if(toCall != NULL)
        toCall(currentJob);

    // Emit signal


    if (commands.isEmpty())
    {
        doingJobs = false;

        emit OnAllJobsFinished();

    }
    else
    {
        DoNextJob();
    }

}

void Elm::DoNextJob()
{
    connect(this,SIGNAL(OnDataRecieve(QByteArray)), this, SLOT(OnJobFinish(QByteArray)));
    connect(this, &SerialDevice::OnMessage, this, &Elm::OnMessageRecieved);
    currentJobCommand = commands.dequeue();
    currentJob.command = currentJobCommand.cmd;
    timeStart = QDateTime::currentMSecsSinceEpoch();
    SendRecieve(currentJobCommand.cmd);
}

void Elm::OnMessageRecieved(QByteArray msg)
{
    if(currentJobCommand.funcToCallAfterMessage != NULL)
    {
        currentJobCommand.funcToCallAfterMessage(msg);
    }
}

QByteArray Elm::ParseResponse(QByteArray da)
{
    return da;
}

void Elm::ChangeBaudRate(qint32 b)
{
    qInfo() << "Changing baud rate...";
    QByteArray a = "STBR";
    a.append(QByteArray::number(b));
    AddJob(a,[=](Job j)
    {
        emit OnDeviceReady();
    }
    ,[=](QByteArray a){
        if (a.contains("OK"))
        {
            qInfo() << "Okay";
            SetBaudRate(b);
            SendRecieve("", true);
        }
    });
}
