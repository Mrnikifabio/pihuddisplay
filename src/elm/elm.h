#ifndef ELM_H
#define ELM_H

#include "serialdevice.h"
#include <QThread>
#include <QString>



class Elm : public SerialDevice
{
    Q_OBJECT
public:

    struct Job
    {
        QByteArray response;
        QByteArray command;
        float execution_time;
    };

    enum DeviceError
    {
        NoError,
        DeviceNotFoundError,
        PermissionError,
        OpenError,
        NotOpenError = 13,
        ParityErrorDepreceated = 4,
        FramingErrorDepreaceated,
        BreakConditionErrorDepreceated,
        WriteError,
        ReadError,
        ResourceError,
        UnsupportedOperation,
        TimeoutError = 12,
        UnknownError = 11,
        BadBaudRate = 14,
        NoDevicesFound = 15

    };



private:
    struct command
    {
        QByteArray cmd;
        std::function<void(Job)> funcToCall;
        std::function<void(QByteArray)> funcToCallAfterMessage;

    };
    QQueue<command> commands;
    command currentJobCommand;
    QTimer simpleTimer;
    Job currentJob;
    void ResetDevice();
    bool doingJobs;
    qint64 timeStart;
    void DoNextJob();
    QByteArray ParseResponse(QByteArray da);
    void GeneralDeviceError(DeviceError error);
signals:
    void OnDeviceReady();
    void OnDeviceFail();
    void OnJobFinished(Elm::Job j);
    void OnAllJobsFinished();
    void OnDeviceFail(Elm::DeviceError);
public:
    Elm();
    void ConnectToDevice(qint32 productID = 24597);
    void ChangeProtocol();
    void AddJob(QByteArray toSend, std::function<void(Job)> func,
                std::function<void(QByteArray)> funcAfterMsg);
    void ChangeBaudRate(qint32 b);
    void AddJob(QByteArray toSend);
    ~Elm();
private slots:
    void OnJobFinish(QByteArray arr);
    void OnSerialPortError(QSerialPort::SerialPortError);
    void OnMessageRecieved(QByteArray msg);
};

#endif // ELM_H
