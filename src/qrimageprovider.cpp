#include "qrimageprovider.h"

#include <QPaintEngine>

QObject* QrImageProvider::m_instance{nullptr};

QrImageProvider::QrImageProvider(QObject *parent)
    : QObject{parent}
{

}

QObject* QrImageProvider::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new QrImageProvider();

    return m_instance;
}


void QrImageProvider::setImage(QImage const &image)
{
    m_image = image;
    emit imageChanged();
}

QImage QrImageProvider::image() const
{
    return m_image;
}

