#ifndef ELM_DEVICE_PRODUCT_ID
#define ELM_DEVICE_PRODUCT_ID
#endif

#include "elmconnection.h"
#include <QtConcurrent/QtConcurrent>

QObject* ELMConnection::m_instance{nullptr};

ELMConnection::ELMConnection(QObject *parent)
    : QObject{parent}
{
    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    m_timer->setSingleShot(true);
    connect(m_timer, &QTimer::timeout, this, &ELMConnection::Reconnect);
    Configure();
}

void ELMConnection::Configure()
{
    m_elm_instance = new Elm();
    connect(m_elm_instance, SIGNAL(OnDeviceReady()), this, SLOT(OnReady()));
    connect(m_elm_instance, SIGNAL(OnDeviceFail(Elm::DeviceError)), this, SLOT(OnFail(Elm::DeviceError)));
    m_elm_instance->ConnectToDevice(ELM_DEVICE_PRODUCT_ID);
}

QObject *ELMConnection::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new ELMConnection();

    return m_instance;
}

void ELMConnection::addJob(const QByteArray& comm, const std::function<void(QByteArray)>& callback)
{
    m_elm_instance->AddJob(comm, nullptr, callback);
}

void ELMConnection::OnReady()
{
    m_elm_instance->AddJob("ATWS", nullptr,[this](const QByteArray& arr){
        qDebug() << "ELM Try to reset";
        if(arr.contains("ELM"))
        {
            qDebug() << "ELM Resetted";
            m_elm_instance->AddJob("AT SP 6", nullptr,

                [this](const QByteArray& arr){
                    qDebug() << QString::fromStdString(arr.toStdString());
                    if(arr.toStdString()=="OK")
                    {
                        qDebug() << QString::fromStdString("OBD Connection Ready");
                        emit ELMConnectionReady();
                    }
                }
            );
        }
    });
}

void ELMConnection::OnFail(Elm::DeviceError error)
{
    qDebug() << "Device failed." << error;
    if(!m_timer->isActive())
        m_timer->start();
}

void ELMConnection::Reconnect()
{
    delete m_elm_instance;
    Configure();
}
