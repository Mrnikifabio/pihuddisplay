#ifndef CARCRUSINGDATAEXPOSER_H
#define CARCRUSINGDATAEXPOSER_H

#define POLLING_INTERVAL 250

#include <QObject>
#include <QTimer>

class CarCruisingDataExposer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int currentValue READ getCurrentValue NOTIFY currentValueChanged)
public:
    explicit CarCruisingDataExposer(QObject *parent = nullptr, unsigned int polling_interval = POLLING_INTERVAL);
    int getCurrentValue();
    void setCurrentValue(int value);

signals:
    void currentValueChanged();

protected:
    bool m_ready_to_poll;

private:
    int m_current_value;
    QTimer *m_poll_timer;
    virtual void poll() = 0;
    void error();
    void ELMConnectionReady();
};

#endif // CARCRUSINGDATAEXPOSER_H
