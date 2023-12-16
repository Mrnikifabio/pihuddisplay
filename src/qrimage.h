#ifndef QRIMAGE_H
#define QRIMAGE_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>

class QrImage : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QImage m_image MEMBER m_image WRITE setImage)
    QImage m_image;
public:
    explicit QrImage(QQuickItem *parent = nullptr);
    void setImage(const QImage& image);
    void paint(QPainter *painter) override;
};

#endif // QRIMAGE_H
