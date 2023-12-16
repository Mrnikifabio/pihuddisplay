#include "speedexposer.h"
#include "elmconnection.h"

QObject* SpeedExposer::m_instance {nullptr};

SpeedExposer::SpeedExposer(QObject *parent)
    : CarCruisingDataExposer(parent){}

QObject *SpeedExposer::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new SpeedExposer();

    return m_instance;
}

void SpeedExposer::poll()
{
    if(m_ready_to_poll)
    {
        m_ready_to_poll = false;
        qDebug() << "Speed Exposer: sent request";
        ((ELMConnection *)ELMConnection::qmlInstance())->addJob("010d",
        [this](const QByteArray& arr){
            //check if data are valid
            try
            {
                auto speedEx = QString::fromStdString(arr.toStdString()).split('\n').last();
                auto speedSplitted = speedEx.split(' ');
                if(speedSplitted.length() >= 3)
                {
                    unsigned int speed = speedSplitted[2].toUInt(nullptr, 16);
                    qDebug() << "Speed " << speed;
                    setCurrentValue(speed);
                }
                else
                    qDebug() << "Speed Exposer:" << speedEx;
            }
            catch(std::exception ex)
            {
                qDebug() << ex.what();
            }

            m_ready_to_poll = true;
        });
    }
}


