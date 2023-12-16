#include "pigpiobutton.h"
#include "qdebug.h"
#include <pigpio.h>
#include <QtConcurrent/QtConcurrent>

bool PiGpioButton::initialized{false};

PiGpioButton::PiGpioButton(QObject *parent) :  m_gpioNumber{-1}, IButton(parent)
{
    if(!initialized)
    {
        gpioCfgInterfaces(PI_DISABLE_SOCK_IF);
        gpioInitialise();
        qInfo() << "pigpiod started";
        initialized = true;
    }
}

PiGpioButton::~PiGpioButton()
{
    qInfo() << "Destroying button";
    gpioSetAlertFuncEx(m_gpioNumber, 0, this);
}

void handleGPIOEvent(int gpio, int level, uint32_t tick, void *user)
{
    if(level == 1)
    {
        qInfo() << "button " << gpio << "pressed" << level;
        emit ((IButton*)(user))->buttonPressedSignal();
    }
}

void PiGpioButton::setGPIONumber(unsigned int gpio)
{
    auto task = QtConcurrent::task([this, gpio](){
        if(m_gpioNumber == -1)
        {
            m_gpioNumber = gpio;
            gpioSetMode(gpio, PI_INPUT);
            gpioSetPullUpDown(gpio, PI_PUD_DOWN);
            qInfo() << ((gpioSetAlertFuncEx(gpio, handleGPIOEvent, this) == 0) ? "GPIO Button event setted" : "Something happened");
            emit gpioNumberChanged();
        }
    }).spawn();
}


unsigned int PiGpioButton::getGPIONumber()
{
    return m_gpioNumber;
}
