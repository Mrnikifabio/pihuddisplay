#include "qrimage.h"

QrImage::QrImage(QQuickItem *parent)
{

}

void QrImage::setImage(const QImage &image)
{
    m_image = image;
    update();
}

void QrImage::paint(QPainter *painter)
{
    painter->drawImage(0,0, m_image);
}
