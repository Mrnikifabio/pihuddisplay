#include "speedmock.h"

QObject* SpeedMock::m_instance {nullptr};

SpeedMock::SpeedMock(QObject *parent)
    : CarCruisingDataExposer{parent, 700}, m_speed{0}
{}

QObject *SpeedMock::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new SpeedMock();

    return m_instance;
}

void SpeedMock::poll()
{
    m_ready_to_poll = false;
    m_speed += 3;
    if(m_speed > 69)
        m_speed = 10;

    setCurrentValue(m_speed);
}
