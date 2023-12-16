#ifndef PIGPIOBUTTON_H
#define PIGPIOBUTTON_H

#include "ibutton.h"

class PiGpioButton : public IButton
{
public:
    explicit PiGpioButton(QObject *parent);
    ~PiGpioButton();

public slots:
    virtual void setGPIONumber(unsigned int gpio) override;
    virtual unsigned int getGPIONumber() override;

private:
    static bool initialized;
    int m_gpioNumber;
};

#endif // PIGPIOBUTTON_H
