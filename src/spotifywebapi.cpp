#define DATA_TIMEOUT 500
#include "spotifywebapi.h"
#include "http/src/http.h"

#include <QByteArray>

QObject* SpotifyWebApi::m_instance{nullptr};

SpotifyWebApi::SpotifyWebApi(QObject *parent)
    : QObject{parent}, m_songId{""}, m_songArtists{""}, m_songTitle{""}, m_songAlbumImage{""},m_currentSongProgress{0}, m_progressBarTotal{0},  m_dataRefreshTimer{new QTimer(this)}
{
    m_blueManagerInstance = ((BluetoothManager*)BluetoothManager::qmlInstance());
    m_authManagerInstance = ((SpotifyAuthManager*)SpotifyAuthManager::qmlInstance());
    connect(m_dataRefreshTimer, &QTimer::timeout, this, &SpotifyWebApi::checkForDataAvailability);
    connect(m_authManagerInstance, &SpotifyAuthManager::tokenReadySignal, this, &SpotifyWebApi::onTokenReady);
    m_dataRefreshTimer->setInterval(DATA_TIMEOUT);
    m_dataRefreshTimer->setSingleShot(false);
    m_dataRefreshTimer->start();
}

SpotifyWebApi::~SpotifyWebApi()
{
    delete m_dataRefreshTimer;
}

QObject *SpotifyWebApi::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new SpotifyWebApi();

    return m_instance;
}

void SpotifyWebApi::requestPlaybackState(AccessToken *accessToken)
{
    if(m_blueManagerInstance->isConnected() && m_authManagerInstance->isConfigured())
    {
        auto http = new Http();
        http->addRequestHeader("Authorization", "Bearer "+accessToken->access_token);
        http->addRequestHeader("Content-Type","application/json");
        http->addRequestHeader("Accept", "application/json");
        auto reply = http->get(QUrl("https://api.spotify.com/v1/me/player"));
        connect(reply, &HttpReply::finished, this, [this, http](auto &reply) {
            if (reply.isSuccessful() && reply.statusCode() == 200) {
                QJsonDocument res = QJsonDocument::fromJson(reply.body());
                auto obj = res.object();
                if(obj["is_playing"].toBool())
                {
                    if(obj["currently_playing_type"].toString() == "track")
                    {
                        if(obj["item"].isObject())
                        {
                            //images/no_album_art__no_cover.jpg
                            auto trackObj = obj["item"].toObject();
                            setProgressBarTotal(trackObj["duration_ms"].toInt());
                            setCurrentSongProgress(obj["progress_ms"].toInt());
                            setSongTitle(trackObj["name"].toString());
                            QString artists;
                            auto artistsArray = trackObj["artists"].toArray();
                            auto count = artistsArray.count();
                            for(int i = 0; i<count; i++)
                            {
                                artists = artistsArray.at(i).toObject()["name"].toString();
                                if(i < count-1)
                                    artists+=", ";
                            }
                            if(trackObj["album"].isObject())
                            {
                                auto album = trackObj["album"].toObject();

                                setSongAlbum(album["name"].toString());

                                if(album["images"].isArray())
                                {
                                    auto albumArray = album["images"].toArray();
                                    if(albumArray.size() > 0)
                                        setSongAlbumImage(albumArray.first().toObject()["url"].toString());
                                }
                            }

                            setSongArtists(artists);
                            setSongId(trackObj["id"].toString());
                            setDataAvailable(true);
                        }
                    }
                    else
                        setDataAvailable(false);
                }
                delete http;
            } else if(reply.statusCode() == 204){
                setDataAvailable(false);
                delete http;
            } else {
                qDebug() << "Player request failed" << reply.statusCode() << QString::fromStdString(reply.body().toStdString());
                setDataAvailable(false);
            }
        });
    }
}

void SpotifyWebApi::checkForDataAvailability()
{
    if(m_blueManagerInstance->isConnected() && m_authManagerInstance->isConfigured())
    {
        if(!m_authManagerInstance->isRequestPending())
            m_authManagerInstance->requestAccessToken();
    }
    else
        setDataAvailable(false);
}

void SpotifyWebApi::onTokenReady(AccessToken *accessToken)
{
    requestPlaybackState(accessToken);
}

void SpotifyWebApi::setSongArtists(QString artists)
{
    if(m_songArtists != artists)
    {
        m_songArtists = artists;
        emit songArtistsChangedSignal();
    }
}

void SpotifyWebApi::setSongTitle(QString title)
{
    if(m_songTitle != title)
    {
        m_songTitle = title;
        emit songTitleChangedSignal();
    }
}

void SpotifyWebApi::setSongAlbumImage(QString songAlbumImage)
{
    if(m_songAlbumImage != songAlbumImage)
    {
        m_songAlbumImage = songAlbumImage;
        emit songAlbumImageChangedSignal();
    }
}

void SpotifyWebApi::setSongAlbum(QString songAlbum)
{
    if(m_songAlbum != songAlbum)
    {
        m_songAlbum = songAlbum;
        emit songAlbumChangedSignal();
    }
}

void SpotifyWebApi::setCurrentSongProgress(int progress)
{
    if(m_currentSongProgress != progress)
    {
        m_currentSongProgress = progress;
        emit currentSongProgressChangedSignal();
    }
}

void SpotifyWebApi::setProgressBarTotal(int total)
{
    if(m_progressBarTotal != total)
    {
        m_progressBarTotal = total;
        emit progressBarTotalChangedSignal();
    }
}

void SpotifyWebApi::setDataAvailable(bool available)
{
    if(available != m_dataAvailable)
    {
        m_dataAvailable = available;
        emit dataAvailableSignal();
    }
}

void SpotifyWebApi::setSongId(const QString& songId)
{
    if(m_songId != songId)
    {
        m_songId = songId;
        emit songIdChangedSignal();
    }
}

QString SpotifyWebApi::getSongTitle()
{
    return m_songTitle;
}

QString SpotifyWebApi::getSongArtists()
{
    return m_songArtists;
}

QString SpotifyWebApi::getSongAlbumImage()
{
    return m_songAlbumImage;
}

QString SpotifyWebApi::getSongAlbum()
{
    return m_songAlbum;
}

QString SpotifyWebApi::getSongId()
{
    return m_songId;
}

int SpotifyWebApi::getCurrentSongProgress()
{
    return m_currentSongProgress;
}

int SpotifyWebApi::getProgressBarTotal()
{
    return m_progressBarTotal;
}

bool SpotifyWebApi::isDataAvailable()
{
    return m_dataAvailable;
}
