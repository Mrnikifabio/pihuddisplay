#ifndef SPEEDMOCK_H
#define SPEEDMOCK_H

#include "carcruisingdataexposer.h"
#include <QQmlEngine>

class SpeedMock : public CarCruisingDataExposer
{
    Q_OBJECT
    Q_DISABLE_COPY(SpeedMock)
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    explicit SpeedMock(QObject *parent = nullptr);

private:
    void poll() override;
    static QObject *m_instance;
    unsigned int m_speed;
};

#endif // SPEEDMOCK_H
