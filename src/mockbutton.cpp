#include "mockbutton.h"
#include <QKeyEvent>

MockButton::MockButton(QObject *parent)
    : IButton{parent}
{
}

void MockButton::installFilter(QObject *obj)
{
    if(obj)
        obj->installEventFilter(this);
}

void MockButton::setGPIONumber(unsigned int gpio)
{
    m_gpio = gpio;
}

unsigned int MockButton::getGPIONumber()
{
    return m_gpio;
}

bool MockButton::eventFilter(QObject *obj, QEvent *event) {
    if (event->type() == QEvent::KeyPress) {
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        if(keyEvent->key() == Qt::Key_Left && m_gpio == 27)
        {
            qDebug("left\n");
            emit buttonPressedSignal();
            return true;
        }
        else
        if(keyEvent->key() == Qt::Key_Right && m_gpio == 22)
        {
            qDebug("right\n");
            emit buttonPressedSignal();
            return true;
        }
        else
        if(keyEvent->key() == Qt::Key_Space && m_gpio == 17)
        {
            qDebug("enter\n");
            emit buttonPressedSignal();
            return true;
        }
        else return false;
    }
    return QObject::eventFilter(obj, event);
}
