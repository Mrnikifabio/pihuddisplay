#include "carcruisingdataexposer.h"
#ifndef CAR_CRUISING_MOCK
    #include "elmconnection.h"
#endif

CarCruisingDataExposer::CarCruisingDataExposer(QObject *parent, unsigned int polling_interval)
    : QObject{parent}, m_poll_timer{new QTimer(this)}, m_ready_to_poll{false}, m_current_value{0}
{
#ifndef CAR_CRUISING_MOCK
    connect(((ELMConnection*)ELMConnection::qmlInstance()), &ELMConnection::ELMConnectionReady, this, &CarCruisingDataExposer::ELMConnectionReady);
#endif
    connect(m_poll_timer, &QTimer::timeout, this, &CarCruisingDataExposer::poll);
    m_poll_timer->setInterval(polling_interval);
    m_poll_timer->setSingleShot(false);
    m_poll_timer->start();
}


void CarCruisingDataExposer::setCurrentValue(int value)
{
    if(m_current_value != value)
    {
        m_current_value = value;
        emit currentValueChanged();
    }
}

void CarCruisingDataExposer::error()
{
    m_ready_to_poll = true;
}

void CarCruisingDataExposer::ELMConnectionReady()
{
    m_ready_to_poll = true;
}

int CarCruisingDataExposer::getCurrentValue()
{
    return m_current_value;
}
