#ifndef QRIMAGEPROVIDER_H
#define QRIMAGEPROVIDER_H

#include <QObject>
#include <QImage>
#include <QQmlEngine>

class QrImageProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QImage image MEMBER m_image READ image WRITE setImage NOTIFY imageChanged)
    Q_DISABLE_COPY(QrImageProvider)
    QImage m_image;
    static QObject *m_instance;
    QrImageProvider(QObject *parent = nullptr);

public:
    static QObject* qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);
    void setImage(QImage const &image);
    QImage image() const;

signals:
    void imageChanged();
};

#endif // QRIMAGEPROVIDER_H
