#include "spotifylyricsapi.h"
#include "spotifywebapi.h"
#include "http/src/http.h"

#include <QRegularExpression>

#define LYRICS_ENDPOINT "http://lrclib.net/api/get"


QObject* SpotifyLyricsApi::m_instance{nullptr};

SpotifyLyricsApi::SpotifyLyricsApi(QObject *parent)
    : QObject{parent}, m_lyricsAvailable{false}, m_currentLinesLyrics{}, m_currentLineLyrics{""}
{
    connect((SpotifyWebApi*)SpotifyWebApi::qmlInstance(), &SpotifyWebApi::songIdChangedSignal, this, &SpotifyLyricsApi::newSongBegunHandle);
    connect((SpotifyWebApi*)SpotifyWebApi::qmlInstance(), &SpotifyWebApi::currentSongProgressChangedSignal, this, &SpotifyLyricsApi::progressChangedHandle);
}

SpotifyLyricsApi::~SpotifyLyricsApi() = default;

QObject *SpotifyLyricsApi::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new SpotifyLyricsApi();

    return m_instance;
}

void SpotifyLyricsApi::newSongBegunHandle()
{
    qDebug() << "FETCHING LYRICS";
    setIsLyricsAvailable(false);
    //fetching lyrics
    auto http = new Http();

    // callback connected to SpotifyWebApi::songIdChangedSignal, we can safely assume that all song infos are updated at this point

    auto webApiInstance = (SpotifyWebApi*)SpotifyWebApi::qmlInstance();
    auto artist = "?artist_name="+QUrl::toPercentEncoding(webApiInstance->getSongArtists());
    auto track_name = "&track_name="+QUrl::toPercentEncoding(webApiInstance->getSongTitle());
    auto album_name = "&album_name="+QUrl::toPercentEncoding(webApiInstance->getSongAlbum());
    auto duration = "&duration="+QString::number(webApiInstance->getProgressBarTotal() / 1000); //ms to sec truncated

    QRegularExpression regex(R"(\[(\d+):(\d+\.\d+)\]\s*(.*))");

    http->setMaxRetries(1);
    http->setReadTimeout(20000);

    qDebug() << QString(LYRICS_ENDPOINT)+artist+track_name+album_name+duration;

    auto reply = http->get(QUrl(QString(LYRICS_ENDPOINT)+artist+track_name+album_name+duration));
    connect(reply, &HttpReply::finished, this, [this, http, regex](auto &reply) {
        if (reply.isSuccessful() && reply.statusCode() == 200) {
            QJsonDocument res = QJsonDocument::fromJson(reply.body());
            auto lyrics = res.object();
            auto syncedLyrics = lyrics["syncedLyrics"].toString("error");
            if(syncedLyrics != "error")
            {
                m_currentLinesLyrics.clear();
                auto lines = syncedLyrics.split("\n");
                for (auto line : lines) {


                    QRegularExpressionMatch match = regex.match(line);
                    if (match.hasMatch()) {
                        auto minutes = match.captured(1).toInt();
                        auto seconds = match.captured(2).toFloat();
                        auto milli = qCeil((minutes * 60 + seconds) * 1000);
                        auto lyric = match.captured(3);
                        m_currentLinesLyrics.push_back({milli, lyric});
                    }
                }

                setIsLyricsAvailable(true);
            }
            else
            {
                qDebug() << "LYRICS FETCH FAILED: ";
                qDebug() << reply.body();
            }
        }
        else
            qDebug() << reply.body();


        delete http;
    });
}

void SpotifyLyricsApi::progressChangedHandle()
{
    if(isLyricsAvailable())
    {
        int currentProgress = ((SpotifyWebApi*)SpotifyWebApi::qmlInstance())->getCurrentSongProgress();
        for(int i = 0; i<m_currentLinesLyrics.size(); i++)
        {
            if(i<m_currentLinesLyrics.size()-1)
            {
                if(currentProgress >= m_currentLinesLyrics.at(i).startTimeMs && currentProgress < m_currentLinesLyrics.at(i+1).startTimeMs)
                {
                    setCurrentLineLyrics(m_currentLinesLyrics.at(i).words);
                    break;
                }
            }
            else
                setCurrentLineLyrics(m_currentLinesLyrics.at(i).words);
        }
    }
}

bool SpotifyLyricsApi::isLyricsAvailable() const
{
    return m_lyricsAvailable;
}

void SpotifyLyricsApi::setIsLyricsAvailable(const bool& isAvailable)
{
    if(m_lyricsAvailable != isAvailable)
    {
        m_lyricsAvailable = isAvailable;
        emit lyricsAvailableChangedSignal();
    }
}

void SpotifyLyricsApi::setCurrentLineLyrics(const QString& newLine)
{
    if(m_currentLineLyrics != newLine)
    {
        m_currentLineLyrics = newLine;
        emit currentLineLyricsChangedSignal();
    }
}

QString SpotifyLyricsApi::getCurrentLineLyrics() const
{
    return m_currentLineLyrics;
}
