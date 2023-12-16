#ifndef UTILS_H
#define UTILS_H

#include <QHostAddress>
#include <QPainter>
#include <QString>

namespace Utils
{
    void executeCommand(const QString& command, const QStringList& args);
    void executeCommand(const QString& command);
    void writeFile(const QString& folderPath, const QString& filePath, std::vector<QString>& fileContent);
    void paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg);
    QHostAddress getIpAddr();
    QString gen_random(const int& len);
    void renewDhcpLease();
}

#endif // UTILS_H
