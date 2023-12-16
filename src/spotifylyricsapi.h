#ifndef SPOTIFYLYRICSAPI_H
#define SPOTIFYLYRICSAPI_H

#include <QObject>
#include <QQmlEngine>

class SpotifyLyricsApi : public QObject
{
    struct Line {
        int startTimeMs;
        QString words;
        int endTimeMs;

        Line(int startTimeMs, const QString words, int endTimeMs)
        {
            this->startTimeMs = startTimeMs;
            this->words = words;
            this->endTimeMs = endTimeMs;
        }
    };

    Q_OBJECT
    Q_DISABLE_COPY(SpotifyLyricsApi)
    Q_PROPERTY(bool available READ isLyricsAvailable NOTIFY lyricsAvailableChangedSignal)
    Q_PROPERTY(QString currentLineLyrics READ getCurrentLineLyrics NOTIFY currentLineLyricsChangedSignal)
    SpotifyLyricsApi(QObject *parent = nullptr);
    ~SpotifyLyricsApi();
private:
    static QObject *m_instance;
    bool m_lyricsAvailable;
    QList<Line*> *m_currentLinesLyrics;
    QString m_currentLineLyrics;
    void setCurrentLineLyrics(const QString& newLine);
    void newSongBegunHandle(const QString& songId);
    void progressChangedHandle();
    bool isLyricsAvailable() const;
    void setIsLyricsAvailable(const bool& isAvailable);

public:
    static QObject *qmlInstance(QQmlEngine *engine = nullptr, QJSEngine *scriptEngine = nullptr);
    QString getCurrentLineLyrics() const;

signals:
    void currentLineLyricsChangedSignal();
    void lyricsAvailableChangedSignal();
};

#endif // SPOTIFYLYRICSAPI_H
