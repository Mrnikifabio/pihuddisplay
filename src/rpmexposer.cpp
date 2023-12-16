#include "rpmexposer.h"
#include "elmconnection.h"

QObject* RPMExposer::m_instance {nullptr};

RPMExposer::RPMExposer(QObject *parent)
    : CarCruisingDataExposer(parent)
{
}

QObject *RPMExposer::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new RPMExposer();

    return m_instance;
}

void RPMExposer::poll()
{
    if(m_ready_to_poll)
    {
        m_ready_to_poll = false;
        qDebug() << "RPM Exposer: sent request";
        ((ELMConnection *)ELMConnection::qmlInstance())->addJob("010c", [this](const QByteArray& arr){
            //check if data are valid
            try
            {
                auto rpmEx = QString::fromStdString(arr.toStdString()).split('\n').last();
                auto rpmSplitted = rpmEx.split(' ');
                if(rpmSplitted.length() >= 4)
                {
                    unsigned int rpm = (rpmSplitted[2]+rpmEx.split(' ')[3]).toUInt(nullptr, 16);
                    qDebug() << "rpm " << rpm / 4;
                    setCurrentValue(rpm/4);
                }
                else
                    qDebug() <<"RPM Exposer: " << rpmEx;
            }
            catch(std::exception ex)
            {
                qDebug() << ex.what();
            }

            m_ready_to_poll = true;
        });
    }
}
