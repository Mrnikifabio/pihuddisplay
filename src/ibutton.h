#ifndef IBUTTON_H
#define IBUTTON_H

#include <QObject>

class IButton : public QObject
{
    Q_OBJECT
    Q_PROPERTY(unsigned int gpioNumber READ getGPIONumber WRITE setGPIONumber NOTIFY gpioNumberChanged)
public:
    explicit IButton(QObject *parent = nullptr) : QObject(parent) {};
    virtual ~IButton() = default;
    Q_INVOKABLE virtual void installFilter(QObject *obj){};

public slots:
    virtual void setGPIONumber(unsigned int gpio) = 0;
    virtual unsigned int getGPIONumber() = 0;

signals:
    void gpioNumberChanged();
    void buttonPressedSignal();
};

#endif // IBUTTON_H
