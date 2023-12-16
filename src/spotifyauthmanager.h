#ifndef SPOTIFYAUTHMANAGER_H
#define SPOTIFYAUTHMANAGER_H

#include "http/src/httpreply.h"
#include <QObject>
#include <QQmlEngine>
#include <QtHttpServer>

struct AccessToken {
    QByteArray access_token;
    QByteArray token_type;
    QByteArray scope;
    int expires_in_secs;
    QByteArray refresh_token;
    QDateTime created;
};

QDataStream& operator<<(QDataStream& out, const AccessToken& v);
QDataStream& operator>>(QDataStream& in, AccessToken& v);

Q_DECLARE_METATYPE(AccessToken);

class SpotifyAuthManager : public QObject
{
private:
    Q_OBJECT
    Q_PROPERTY(bool authenticated READ isAuthenticated WRITE setIsAuthenticated NOTIFY authenticationChangedSignal)
    Q_PROPERTY(bool configured READ isConfigured WRITE setIsConfigured NOTIFY configurationChangedSignal)
    Q_DISABLE_COPY(SpotifyAuthManager)
    SpotifyAuthManager(QObject *parent = nullptr);

    static QObject *m_instance;
    QHttpServer *m_server;
    bool m_authenticated;
    bool m_requestPending;
    bool m_configured;
    AccessToken *m_access_token;
    void renewAccessToken(AccessToken *old = nullptr);
    void setRequestPending(bool pending);
    void callback1(const HttpReply& reply);
    void callback2(const HttpReply& reply);

public:
    static QObject *qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);
    bool isAuthenticated();
    bool isRequestPending();
    bool isConfigured();
    Q_INVOKABLE void beginAuthProcess();
    Q_INVOKABLE void endAuthProcess();
    Q_INVOKABLE void clearSettings();
    void requestAccessToken();

public slots:
    void setIsAuthenticated(bool isAuthenticated);
    void setIsConfigured(bool isConfigured);

signals:
    void authenticationStartedSignal();
    void authenticationFinishedSignal();
    void authenticationChangedSignal();
    void authenticationFailedSignal();
    void configurationChangedSignal();
    void tokenReadySignal(AccessToken* token);
};

#endif // SPOTIFYAUTHMANAGER_H
