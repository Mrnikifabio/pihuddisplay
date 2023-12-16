#include "rpmmock.h"

QObject* RPMMock::m_instance {nullptr};

RPMMock::RPMMock(QObject *parent)
    : CarCruisingDataExposer{parent, 450}, m_rpm{0}
{}


QObject *RPMMock::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new RPMMock();

    return m_instance;
}

void RPMMock::poll()
{

    m_rpm += 200;
    if(m_rpm > 2800)
        m_rpm = 1200;

    setCurrentValue(m_rpm);
}
