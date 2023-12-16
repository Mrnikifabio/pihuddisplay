#ifndef RPMEXPOSER_H
#define RPMEXPOSER_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>
#include "carcruisingdataexposer.h"


class RPMExposer : public CarCruisingDataExposer
{
    Q_OBJECT
    Q_DISABLE_COPY(RPMExposer)
    RPMExposer(QObject *parent = nullptr);
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);

private:
    void poll() override;
    static QObject *m_instance;
};


#endif // RPMEXPOSER_H
