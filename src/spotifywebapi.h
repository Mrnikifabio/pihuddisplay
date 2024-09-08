#ifndef SPOTIFYWEBAPI_H
#define SPOTIFYWEBAPI_H

#include <QObject>
#include <QQmlEngine>
#include <QTimer>

#include "bluetoothmanager.h"
#include "spotifyauthmanager.h"

class SpotifyWebApi : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(SpotifyWebApi)
    Q_PROPERTY(QString songTitle READ getSongTitle NOTIFY songTitleChangedSignal)
    Q_PROPERTY(QString songArtists READ getSongArtists NOTIFY songArtistsChangedSignal)
    Q_PROPERTY(QString songAlbumImage READ getSongAlbumImage NOTIFY songAlbumImageChangedSignal)
    Q_PROPERTY(QString songAlbum READ getSongAlbum NOTIFY songAlbumChangedSignal)
    Q_PROPERTY(QString songId READ getSongId NOTIFY songIdChangedSignal)
    Q_PROPERTY(int currentSongProgress READ getCurrentSongProgress NOTIFY currentSongProgressChangedSignal)
    Q_PROPERTY(int progressBarTotal READ getProgressBarTotal NOTIFY progressBarTotalChangedSignal)
    Q_PROPERTY(bool dataAvailable READ isDataAvailable NOTIFY dataAvailableSignal)
    SpotifyWebApi(QObject *parent = nullptr);
    ~SpotifyWebApi();
private:
    static QObject *m_instance;
    QTimer *m_dataRefreshTimer;
    QString m_songTitle;
    QString m_songArtists;
    QString m_songAlbumImage;
    QString m_songId;
    QString m_songAlbum;
    int m_currentSongProgress;
    int m_progressBarTotal;
    SpotifyAuthManager* m_authManagerInstance;
    BluetoothManager* m_blueManagerInstance;
    bool m_dataAvailable;
    void setSongArtists(QString artists);
    void setSongTitle(QString title);
    void setSongAlbumImage(QString songAlbumImage);
    void setSongAlbum(QString songAlbum);
    void setCurrentSongProgress(int progress);
    void setProgressBarTotal(int total);
    void setDataAvailable(bool available);
    void setSongId(const QString& songId);
    void checkForDataAvailability();
    void onTokenReady(AccessToken *accessToken);
    void requestPlaybackState(AccessToken *accessToken);

public:
    static QObject *qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);
    QString getSongTitle();
    QString getSongArtists();
    QString getSongAlbumImage();
    QString getSongAlbum();
    QString getSongId();
    int getCurrentSongProgress();
    int getProgressBarTotal();
    bool isDataAvailable();

signals:
    void songArtistsChangedSignal();
    void songTitleChangedSignal();
    void songAlbumImageChangedSignal();
    void currentSongProgressChangedSignal();
    void progressBarTotalChangedSignal();
    void songAlbumChangedSignal();
    void dataAvailableSignal();
    void songIdChangedSignal();
};

#endif // SPOTIFYWEBAPI_H
