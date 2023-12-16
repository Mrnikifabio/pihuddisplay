#ifndef CLOCKEXPOSER_H
#define CLOCKEXPOSER_H

#include <QObject>
#include <QQmlEngine>
#include <QtConcurrent/QtConcurrent>

class ClockExposer : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(ClockExposer)
    Q_PROPERTY(int currentHour READ getCurrentHour WRITE setCurrentHour NOTIFY currentHourChanged)
    Q_PROPERTY(int currentMinute READ getCurrentMinute WRITE setCurrentMinute NOTIFY currentMinuteChanged)
    ClockExposer(QObject *parent = nullptr);
public:
    static QObject *qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine);
    int getCurrentHour();
    int getCurrentMinute();

signals:
    void currentHourChanged();
    void currentMinuteChanged();

public slots:
    void setCurrentHour(int hour);
    void setCurrentMinute(int minute);
    void syncTime();

private:
    int m_current_hour;
    int m_current_minute;
    QTimer *m_sync_timer;
    static QObject *m_instance;
};
#endif // CLOCKEXPOSER_H
