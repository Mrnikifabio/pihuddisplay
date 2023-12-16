#ifndef SPEEDEXPOSER_H
#define SPEEDEXPOSER_H

#include <QObject>
#include <QQmlEngine>
#include "carcruisingdataexposer.h"

class SpeedExposer : public CarCruisingDataExposer
{
    Q_OBJECT
    Q_DISABLE_COPY(SpeedExposer)
    SpeedExposer(QObject *parent = nullptr);
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

private:
    static QObject *m_instance;
    void poll() override;
};

#endif // SPEEDEXPOSER_H
