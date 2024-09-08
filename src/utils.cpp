#include "utils.h"

#include <QProcess>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QNetworkInterface>
#include <QDBusInterface>
#include <QDBusReply>
#include "qrcodegen.h"

void Utils::executeCommand(const QString& command, const QStringList& args)
{
    QProcess process;
    process.start(command,args);
    process.waitForFinished(-1);
    qDebug() << process.readAllStandardOutput();
}

void Utils::executeCommand(const QString& command)
{
    QProcess process;
    process.start(command);
    process.waitForFinished(-1);
    qDebug() << process.readAllStandardOutput();
}

void Utils::writeFile(const QString& folderPath, const QString& filePath, std::vector<QString>& fileContent)
{
    QDir dir;
    if(!dir.exists(folderPath))
        dir.mkpath(folderPath);
    QFile file(folderPath+filePath);
    if(!file.exists())
    {
        if(file.open(QIODevice::WriteOnly))
        {
            QTextStream stream( &file );
            for(QString& str : fileContent)
                stream << str << Qt::endl;
        }

        file.close();
    }
}

void Utils::paintQR(QPainter &painter, const QSize sz, const QString &data, QColor fg)
{
    // NOTE: At this point you will use the API to get the encoding and format you want, instead of my hardcoded stuff:
    qrcodegen::QrCode qr = qrcodegen::QrCode::encodeText(data.toUtf8().constData(), qrcodegen::QrCode::Ecc::LOW);
    const int s=qr.getSize()>0?qr.getSize():1;
    const double w=sz.width();
    const double h=sz.height();
    const double aspect=w/h;
    const double size=((aspect>1.0)?h:w);
    const double scale=size/(s+2);
    // NOTE: For performance reasons my implementation only draws the foreground parts in supplied color.
    // It expects background to be prepared already (in white or whatever is preferred).
    painter.setPen(Qt::NoPen);
    painter.setBrush(fg);
    for(int y=0; y<s; y++) {
        for(int x=0; x<s; x++) {
            const int color=qr.getModule(x, y);  // 0 for white, 1 for black
            if(0!=color) {
                const double rx1=(x+1)*scale, ry1=(y+1)*scale;
                QRectF r(rx1, ry1, scale, scale);
                painter.drawRects(&r,1);
            }
        }
    }
}

QHostAddress Utils::getIpAddr()
{
    QNetworkInterface bnep0 = QNetworkInterface::interfaceFromName("bnep0");
    QList<QNetworkAddressEntry> entries = bnep0.addressEntries();
    if (!entries.isEmpty()) {
        QNetworkAddressEntry entry = entries.first();
        return entry.ip();
    }
    return QHostAddress{};
}

QString Utils::gen_random(const int& len) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";
    QString tmp_s;
    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i) {
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    }

    return tmp_s;
}

void Utils::renewDhcpLease()
{
    qDebug() << "Leasing ip";
    QProcess dhcpLeaseCommand;
    dhcpLeaseCommand.start("udhcpc",{"-i", "bnep0"});
    dhcpLeaseCommand.waitForFinished(-1);
    auto code = dhcpLeaseCommand.exitCode();
    qDebug() << "Leased ip, exit code: "<< code;

    if(code != 0)
        qDebug() << "IP LEASING FAILED: check that UDHCPC is available and working";
 }
