#include "spotifyauthmanager.h"
#include "qrimageprovider.h"
#include "utils.h"
#include "bluetoothmanager.h"
#include <QColor>
#include <QPaintEngine>
#include <http/src/http.h>

#define STRINGIZE(x) #x
#define STRINGIZE_VALUE_OF(x) STRINGIZE(x)

#ifdef HTTP_SERVER_PORT
#define HTTP_SERVER_PORT 8080
#endif

#ifndef CLIENTID_SECRET
#define CLIENTID_SECRET
#endif

#ifndef CLIENTID
#define CLIENTID
#endif

QDataStream& operator<<(QDataStream& out, const AccessToken& v) {
    out << v.access_token << v.token_type << v.scope << v.expires_in_secs << v.refresh_token << v.created;
    return out;
}

QDataStream& operator>>(QDataStream& in, AccessToken& v) {
    in >> v.access_token;
    in >> v.token_type;
    in >> v.scope;
    in >> v.expires_in_secs;
    in >> v.refresh_token;
    in >> v.created;
    return in;
}


QObject* SpotifyAuthManager::m_instance{nullptr};

SpotifyAuthManager::SpotifyAuthManager(QObject *parent)
    : QObject{parent}, m_access_token{nullptr}, m_server{nullptr}
{
    QSettings settings;

    if(!settings.contains("spotify/token"))
    {
        m_authenticated = false;
        if(settings.contains("spotify/code") && settings.contains("spotify/redirect_uri"))
            m_configured = true;
    }
    else
    {
        auto temp = settings.value("spotify/token").value<AccessToken>();
        m_access_token = new AccessToken();
        m_access_token->access_token = temp.access_token;
        m_access_token->refresh_token = temp.refresh_token;
        m_access_token->token_type = temp.token_type;
        m_access_token->created = temp.created;
        m_access_token->expires_in_secs = temp.expires_in_secs;
        m_access_token->scope = temp.scope;
        m_authenticated = true;
        m_configured = true;
    }
}

QObject *SpotifyAuthManager::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new SpotifyAuthManager();

    return m_instance;
}

bool SpotifyAuthManager::isAuthenticated()
{
    return m_authenticated;
}

bool SpotifyAuthManager::isRequestPending()
{
    return m_requestPending;
}

bool SpotifyAuthManager::isConfigured()
{
    return m_configured;
}

void SpotifyAuthManager::beginAuthProcess()
{
    setIsConfigured(false);
    setIsAuthenticated(false);

    if(m_server != nullptr)
    {
        delete m_server;
        m_server = nullptr;
    }

    m_server = new QHttpServer();
    if(((BluetoothManager*)BluetoothManager::qmlInstance())->isConnected())
    {
        auto ip = Utils::getIpAddr().toString();
        auto url = "https://accounts.spotify.com/authorize?client_id="+QString(STRINGIZE_VALUE_OF(CLIENTID))+"&scope=user-read-playback-state&response_type=code&redirect_uri=http://"+ip+":8080/api/spotify/auth&state="+Utils::gen_random(QRandomGenerator().bounded(30));

        qDebug() << "redirect: " << "http://"+ip+":8080/api/spotify/auth";

        m_server->route("/api/spotify/auth", QHttpServerRequest::Method::Get, [this, url, ip](const QHttpServerRequest& req){
            if(!isConfigured() && !isAuthenticated())
            {

                auto queryItems = QUrlQuery(req.url()).queryItems();
                return QtConcurrent::run([this, url, ip, queryItems](){
                    for(auto& value : queryItems)
                    {
                        if(value.first == "code")
                        {
                            QSettings settings;
                            settings.setValue("spotify/code", value.second);
                            settings.setValue("spotify/redirect_uri", "http://"+ip+":8080/api/spotify/auth");
                            setIsConfigured(true);
                        }
                        else if(value.first == "error")
                        {
                            qDebug() << value.second << " " << value.second;
                            setIsConfigured(false);
                        }
                    }

                    auto res = QHttpServerResponse{QHttpServerResponder::StatusCode::MovedPermanently};
                    res.setHeader("Location","https://open.spotify.com");
                    if(isConfigured())
                        emit authenticationFinishedSignal();
                    else
                        emit authenticationFailedSignal();

                    return res;
                });

            }
            else
            {
                return QtConcurrent::run([this](){
                    auto res = QHttpServerResponse{QHttpServerResponder::StatusCode::MovedPermanently};
                    res.setHeader("Location","https://open.spotify.com");
                    return res;
                });
            }
        });

        if(m_server->listen(QHostAddress::Any, HTTP_SERVER_PORT) == HTTP_SERVER_PORT)
        {
            auto task = QtConcurrent::task([this, ip, url](){
                QImage img {200, 200, QImage::Format_ARGB32};
                img.fill(QColor(255,255,255));
                QPainter painter(&img);
                if(!ip.isNull())
                {
                    Utils::paintQR(painter, img.size(), url, QColor("black"));
                    QMetaObject::invokeMethod(QCoreApplication::instance(), [img] { //run on main thread
                        ((QrImageProvider *)QrImageProvider::qmlInstance())->setImage(std::move(img));
                    }, Qt::BlockingQueuedConnection);
                    emit authenticationStartedSignal();
                }
                else emit authenticationFailedSignal();

            }).spawn();
        }
    }
    else emit authenticationFailedSignal();
}

void SpotifyAuthManager::endAuthProcess()
{
    emit authenticationFinishedSignal();
}

void SpotifyAuthManager::clearSettings()
{
    QSettings settings;
    settings.remove("spotify/code");
    settings.remove("spotify/redirect_uri");
    settings.remove("spotify/token");
    delete m_access_token;
    setIsAuthenticated(false);
    setIsConfigured(false);
}

void SpotifyAuthManager::requestAccessToken()
{
    setRequestPending(true);
    if(((BluetoothManager*)BluetoothManager::qmlInstance())->isConnected() && isConfigured())
    {
        if(m_access_token != nullptr)
        {
            if((m_access_token->created.secsTo(QDateTime::currentDateTime())) > m_access_token->expires_in_secs)
                renewAccessToken(m_access_token);
            else
            {
                emit tokenReadySignal(m_access_token);
                setRequestPending(false);
            }
        }
        else
            renewAccessToken();
    }
    else
        setRequestPending(false);

    if(m_access_token != nullptr)
        qDebug() << QString::fromStdString( m_access_token->access_token.toStdString());
}


void SpotifyAuthManager::renewAccessToken(AccessToken *old)
{
    if(((BluetoothManager*)BluetoothManager::qmlInstance())->isConnected() && isConfigured())
    {
        QSettings settings;
        auto code = settings.value("spotify/code").toString();
        auto redirect = settings.value("spotify/redirect_uri").toString();
        if(old == nullptr)
        {
            auto params = QMap<QString, QString>();
            params.insert("grant_type","authorization_code");
            params.insert("code", code);
            params.insert("redirect_uri", redirect);
            auto m_http = new Http();
            auto encoded = QByteArray(STRINGIZE_VALUE_OF(CLIENTID_SECRET));
            m_http->addRequestHeader("Authorization", "Basic "+encoded.toBase64());
            auto reply = m_http->post(QUrl("https://accounts.spotify.com/api/token"), params);
            connect(reply, &HttpReply::finished, this, &SpotifyAuthManager::callback1);
        }
        else
        {
            auto params = QMap<QString, QString>();
            params.insert("grant_type","refresh_token");
            params.insert("refresh_token", old->refresh_token);
            auto m_http = new Http();
            auto encoded = QByteArray(STRINGIZE_VALUE_OF(CLIENTID_SECRET));
            m_http->addRequestHeader("Authorization", "Basic "+encoded.toBase64());
            auto reply = m_http->post(QUrl("https://accounts.spotify.com/api/token"), params);
            connect(reply, &HttpReply::finished, this, &SpotifyAuthManager::callback2);
        }
    }
}

void SpotifyAuthManager::setRequestPending(bool pending)
{
    if(pending!=m_requestPending)
    {
        m_requestPending = pending;
    }
}

void SpotifyAuthManager::callback1(const HttpReply &reply)
{
    if (reply.isSuccessful() && reply.statusCode() == 200) {
        AccessToken *temp = new AccessToken();
        qDebug() << "Token renewed";
        QJsonDocument res = QJsonDocument::fromJson(reply.body());
        auto obj = res.object();
        temp->access_token = obj["access_token"].toString().toUtf8();
        temp->token_type = obj["token_type"].toString().toUtf8();
        temp->scope = obj["scope"].toString().toUtf8();
        temp->expires_in_secs = obj["expires_in"].toInt();
        temp->refresh_token = obj["refresh_token"].toString().toUtf8();
        temp->created = QDateTime::currentDateTime();
        this->m_access_token = temp;
        QSettings settings;
        settings.setValue("spotify/token", QVariant::fromValue(*m_access_token));
        setIsAuthenticated(true);
        emit tokenReadySignal(temp);
    } else {
        qDebug() << "Token request failed" << reply.statusCode() << reply.reasonPhrase();
        setIsAuthenticated(false);
    }
    setRequestPending(false);
}

void SpotifyAuthManager::callback2(const HttpReply &reply)
{
    if (reply.isSuccessful() && reply.statusCode() == 200) {
        qDebug() << "Token renewed";
        QJsonDocument res = QJsonDocument::fromJson(reply.body());
        auto obj = res.object();
        this->m_access_token->access_token = obj["access_token"].toString().toUtf8();
        this->m_access_token->token_type = obj["token_type"].toString().toUtf8();
        this->m_access_token->scope = obj["scope"].toString().toUtf8();
        this->m_access_token->expires_in_secs = obj["expires_in"].toInt();
        this->m_access_token->created = QDateTime::currentDateTime();
        QSettings settings;
        settings.setValue("spotify/token", QVariant::fromValue(*m_access_token));
        setIsAuthenticated(true);
        emit tokenReadySignal(this->m_access_token);
    } else {
        qDebug() << "Token request failed" << reply.statusCode() << reply.reasonPhrase();
        setIsAuthenticated(false);
    }
    setRequestPending(false);
}

void SpotifyAuthManager::setIsAuthenticated(bool isAuthenticated)
{
    if(isAuthenticated != m_authenticated)
    {
        m_authenticated = isAuthenticated;
        emit authenticationChangedSignal();
    }
}

void SpotifyAuthManager::setIsConfigured(bool isConfigured)
{
    if(isConfigured != m_configured)
    {
        m_configured = isConfigured;
        emit configurationChangedSignal();
    }
}
