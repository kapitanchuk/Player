#ifndef AUDIOFILE_H
#define AUDIOFILE_H
#include <QMediaContent>
#include <QMediaResource>
#include <QDebug>
#include <QFileInfo>
#include <QAudioBuffer>
#include <QAudioFormat>
#include <ctime>
#include <Windows.h>
#include <players.h>
#include <QMediaPlayer>
#include <random>
#include <QObject>
#include <QTimer>
#include <QFileDialog>

class AudioFile:public QMediaContent
{
    enum Properties{
        AudioCodec,
        SampleRate,
        AudioBitRate,
        Duration,
    };

    QString filePath;
    QFileInfo fileInfo;
    QString name;
    QString format;
    double duration;
    QString audioCodec;
    bool caraokeText;
    int sampleRate;//Частота дискретизації
    qint64 bitRate;
    QString player;
    void getMetaData();

    void onMediaStatusChanged();
    Properties resolveProperties(const QString& string);
    QMediaPlayer *player_ = new QMediaPlayer();
public:
    QString getName()const{return this->name;};
    QString getFormat()const{return this->format;};
    double getDuration()const{return this->duration;};
    QString getAudioCodec()const{return this->audioCodec;};
    bool getCaraokeText()const{return this->caraokeText;};
    int getSampleRate()const{return this->sampleRate;};
    QString getFilePath()const{return this->filePath;};
    qint64 getBitRate() const{return this->bitRate;};
    QString getPlayer() const{return this->player;};

    friend void operator<<(QFile &file_,const AudioFile&file);
    friend void operator>>(QFileDialog&dialog,AudioFile&file);

    AudioFile();
    AudioFile(QString filePath);
    AudioFile(const QUrl &contentUrl);
    AudioFile(const AudioFile &file);
    ~AudioFile();

};

#endif // AUDIOFILE_H
