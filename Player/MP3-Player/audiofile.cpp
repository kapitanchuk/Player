#include "audiofile.h"
#include <QMap>

AudioFile::AudioFile():QMediaContent()
{

}

AudioFile::AudioFile(QString filePath):QMediaContent(QUrl::fromLocalFile(filePath))
{
    this->filePath = filePath;
}

AudioFile::AudioFile( const QUrl &contentUrl):QMediaContent(contentUrl){
    filePath = contentUrl.path();
    fileInfo.setFile(filePath);

    name = fileInfo.baseName();
    format = fileInfo.suffix();
    player = players[rand()%5];
    caraokeText = rand()%2;

    QObject::connect(player_,&QMediaPlayer::mediaStatusChanged,[this](){onMediaStatusChanged();});
    player_->setMedia(contentUrl);
}

AudioFile::AudioFile(const AudioFile &file):AudioFile(QUrl::fromLocalFile(file.getFilePath()))
{

}

void operator<<(QFile &file_,const AudioFile&file){
    QString string = "Information about "+file.getName()+file.getFormat()+": Bit Rate: "+QString::number(file.getBitRate())+", SampleRate: "+QString::number(file.getSampleRate())+"\n";
    file_.write(string.toStdString().c_str());
}

void operator>>(QFileDialog&dialog,AudioFile&file){
    QString string = dialog.getOpenFileName(nullptr,"Select Files", "C:\\_programmer\\univer\\2_course\\1_sem\\course-project\\music");
    file = *new AudioFile(QUrl::fromLocalFile(string));
}

void AudioFile::onMediaStatusChanged()
{
    qDebug()<<player_->mediaStatus();
    if(player_->mediaStatus()== QMediaPlayer::LoadedMedia){
        getMetaData();
    }
}

AudioFile::Properties AudioFile::resolveProperties(const QString &string)
{
    const QMap<QString,Properties> PropertiesMap{
        {"AudioBitRate",AudioBitRate},
        {"AudioCodec",AudioCodec},
        {"Duration",Duration},
        {"SampleRate",SampleRate}
    };
    auto itr = PropertiesMap.find(string);
    return itr.value();
}

void AudioFile::getMetaData()
{
    QStringList metaDataList = player_->availableMetaData();

    for(QString key:metaDataList){

        switch(resolveProperties(key)){
        case AudioBitRate:
            bitRate = player_->metaData(key).toInt();
            break;
        case AudioCodec:
            audioCodec = player_->metaData(key).toString();
            break;
        case Duration:
            duration = player_->metaData(key).toDouble()/(60*1000);
            break;
        case SampleRate:
            sampleRate = player_->metaData(key).toInt();
            break;
        default:
            break;
        }
    }
}

AudioFile::~AudioFile()
{
    delete player_;
}

