#ifndef ELMCONNECTION_H
#define ELMCONNECTION_H

#include <QObject>
#include <QQmlEngine>
#include "elm/elm.h"

class ELMConnection : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ELMConnection)
    ELMConnection(QObject *parent = nullptr);
private:
    static QObject *m_instance;
    Elm *m_elm_instance;
    QTimer *m_timer;
    void Configure();

public:
    static QObject *qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);
    void addJob(const QByteArray& comm, const std::function<void(QByteArray)>& callback);

private slots:
    void OnReady();
    void OnFail(Elm::DeviceError error);
    void Reconnect();

signals:
    void ELMConnectionReady();
};

#endif // ELMCONNECTION_H
