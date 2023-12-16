#include "clockexposer.h"

QObject* ClockExposer::m_instance {nullptr};

ClockExposer::ClockExposer(QObject *parent)
    : QObject{parent}
    , m_current_hour{0}, m_current_minute{0}, m_sync_timer{new QTimer{this}}
{
    connect(m_sync_timer, &QTimer::timeout, this, &ClockExposer::syncTime);
    m_sync_timer->start(500);
}

QObject *ClockExposer::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new ClockExposer();

    return m_instance;
}

int ClockExposer::getCurrentHour()
{
    return m_current_hour;
}

int ClockExposer::getCurrentMinute()
{
    return m_current_minute;
}

void ClockExposer::setCurrentHour(int hour)
{
    if(hour!= m_current_hour)
    {
        m_current_hour = hour;
        emit currentHourChanged();
    }
}

void ClockExposer::setCurrentMinute(int minute)
{

    if(minute!= m_current_minute)
    {
        m_current_minute = minute;
        emit currentMinuteChanged();
    }
}

void ClockExposer::syncTime()
{
    QTime current = QDateTime::currentDateTime().toLocalTime().time();
    setCurrentHour(current.hour());
    setCurrentMinute(current.minute());
}


