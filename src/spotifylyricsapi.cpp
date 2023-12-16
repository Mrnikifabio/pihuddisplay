#include "spotifylyricsapi.h"
#include "spotifywebapi.h"
#include "http/src/http.h"

#define LYRICS_ENDPOINT

QObject* SpotifyLyricsApi::m_instance{nullptr};

SpotifyLyricsApi::SpotifyLyricsApi(QObject *parent)
    : QObject{parent}, m_lyricsAvailable{false}, m_currentLinesLyrics{nullptr}, m_currentLineLyrics{""}
{
    connect((SpotifyWebApi*)SpotifyWebApi::qmlInstance(), &SpotifyWebApi::songIdChangedSignal, this, &SpotifyLyricsApi::newSongBegunHandle);
    connect((SpotifyWebApi*)SpotifyWebApi::qmlInstance(), &SpotifyWebApi::currentSongProgressChangedSignal, this, &SpotifyLyricsApi::progressChangedHandle);
}

SpotifyLyricsApi::~SpotifyLyricsApi()
{
    delete m_currentLinesLyrics;
}

QObject *SpotifyLyricsApi::qmlInstance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);

    if (m_instance == nullptr)
        m_instance = new SpotifyLyricsApi();

    return m_instance;
}

void SpotifyLyricsApi::newSongBegunHandle(const QString& songId)
{
    setIsLyricsAvailable(false);
    //fetching lyrics
    auto http = new Http();
    auto reply = http->get(QUrl(QString(LYRICS_ENDPOINT)+songId+"?autoplay=true"));
    connect(reply, &HttpReply::finished, this, [this, http](auto &reply) {
        if (reply.isSuccessful() && reply.statusCode() == 200) {
            QJsonDocument res = QJsonDocument::fromJson(reply.body());
            auto lyrics = res.object();
            bool error = true;
            error = lyrics["error"].toBool();
            if(!error)
            {
                if(lyrics["syncType"].toString() == "LINE_SYNCED")
                {
                    if(lyrics["lines"].isArray())
                    {
                        if(m_currentLineLyrics != nullptr)
                            delete m_currentLinesLyrics;
                        auto lines = lyrics["lines"].toArray();
                        m_currentLinesLyrics = new QList<Line*>();
                        auto count = lines.count();
                        for(int i = 0; i<count; i++)
                        {
                            auto line = lines.at(i).toObject();
                            m_currentLinesLyrics->push_back(new Line(line["startTimeMs"].toString().toInt(), line["words"].toString(), line["endTimeMs"].toString().toInt()));
                        }
                        setIsLyricsAvailable(true);
                    }
                }
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
        for(int i = 0; i<m_currentLinesLyrics->size(); i++)
        {
            if(i<m_currentLinesLyrics->size()-1)
            {
                if(currentProgress >= m_currentLinesLyrics->at(i)->startTimeMs && currentProgress < m_currentLinesLyrics->at(i+1)->startTimeMs)
                {
                    setCurrentLineLyrics(m_currentLinesLyrics->at(i)->words);
                    break;
                }
            }
            else
                setCurrentLineLyrics(m_currentLinesLyrics->at(i)->words);
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
