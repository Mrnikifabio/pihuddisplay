#ifndef RPMMOCK_H
#define RPMMOCK_H

#include "carcruisingdataexposer.h"
#include <QQmlEngine>

class RPMMock : public CarCruisingDataExposer
{
    Q_OBJECT
    Q_DISABLE_COPY(RPMMock)
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    explicit RPMMock(QObject *parent = nullptr);

private:
    void poll() override;
    static QObject *m_instance;
    unsigned int m_rpm;
};

#endif // RPMMOCK_H
