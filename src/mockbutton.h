#ifndef MOCKBUTTON_H
#define MOCKBUTTON_H

#include "ibutton.h"

class MockButton : public IButton
{
public:
    explicit MockButton(QObject *parent = nullptr);
    ~MockButton() = default;
    void installFilter(QObject *obj) override;

public slots:
    virtual void setGPIONumber(unsigned int gpio) override;
    virtual unsigned int getGPIONumber() override;

protected:
    bool eventFilter(QObject *obj, QEvent *event) override;

private:
    unsigned int m_gpio;
};

#endif // MOCKBUTTON_H
