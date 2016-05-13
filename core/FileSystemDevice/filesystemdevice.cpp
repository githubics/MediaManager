/*
* Copyright (C) 2016  Integrated Computer Solutions, Inc
* 54 B Middlesex Turnpike, Bedford, MA 01730
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License
* as published by the Free Software Foundation; either version 2
* of the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#if ! defined QT_NO_DEBUG_OUTPUT
#define QT_NO_DEBUG_OUTPUT
#endif
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QDir>
#include <QUrl>
#include <QProcess>
#include "filesystemdevice.h"

#if defined USE_MEDIAINFO
#include "MediaInfo/MediaInfoList.h"
#define QSLWSTR( _ARG_ ) QStringLiteral( _ARG_ ).toStdWString()
#endif

// TODO:
// This is ready to be moved into a QThread now.
class IndexingWorker : public QObject
{
public:
    IndexingWorker(QObject * parent=0)
        : QObject(parent) {
        // TODO: These need to be synced with the capabilities of the corresponding
        // players somehow.
        // Maybe define a set of AudioFileMediaTypes in the MediaManager and use them here.
        audioFilters << "*.mp3" << "*.ogg" << "*.wav";
        videoFilters << "*.mp4" << "*.mkv" << "*.m4v" << "*.avi" << "*.wmv" << "*.mov";
    }
    ~IndexingWorker(){}

    void startIndexing(const QUrl url);

    QJsonArray audioFiles() const {return m_audioFiles;}
    QJsonArray videoFiles() const {return m_videoFiles;}
private:
    void indexDirectory(const QString dirPath);
    void mediaInfo(const QStringList fileList);

private:
    QJsonArray m_audioFiles;
    QJsonArray m_videoFiles;
    QStringList audioFilters;
    QStringList videoFilters;
};

void IndexingWorker::startIndexing(const QUrl url)
{
    m_audioFiles = QJsonArray();
    m_videoFiles = QJsonArray();
    indexDirectory(url.toLocalFile());
}


void IndexingWorker::mediaInfo(const QStringList fileList)
{
    if (fileList.isEmpty()) return;
    QStringList generalParams;
    generalParams << "CompleteName" << "FolderName" << "FileName" << "FileExtension" << "Artist" << "Cover_Data"
                  << "FileSize" << "Duration" << "FrameRate" << "BitRate" << "BitRate_Mode/String"
                  << "Format" << "InternetMediaType"
                  << "Title" << "Season" << "Movie"
                  << "Album" << "Album_More" << "Album/Performer"
                  << "Part" << "Track" << "Track/Position" << "Compilation"
                  << "Performer" << "Genre" << "Mood"
                  << "Released_Date" << "Original/Released_Date" << "Recorded_Date"
                  << "Encoded_Application" << "Encoded_Library" << "BPM"
                  << "Cover" << "Cover_Mime" << "Lyrics"
                  << "Added_Date";
    QString generalInform;
    generalInform="General;";
    foreach(QString s, generalParams) {
        generalInform += QString("\%%1\%|").arg(s);
    }
    generalInform+="\\n";
    MediaInfoLib::MediaInfoList MI;
    MI.Option(QSLWSTR("ParseSpeed"), QSLWSTR("0"));
    MI.Option(QSLWSTR("Language"), QSLWSTR("raw"));
    MI.Option(QSLWSTR("ReadByHuman"), QSLWSTR("0"));
    MI.Option(QSLWSTR("Legacy"), QSLWSTR("0"));
    int nfiles;
    foreach (QString file, fileList) {
        nfiles=MI.Open(file.toStdWString(), MediaInfoLib::FileOption_NoRecursive);
    }
    qDebug() << Q_FUNC_INFO << "Opened" << nfiles << "files";
    if (nfiles!=fileList.count()) {
        qWarning() << Q_FUNC_INFO << "some files could not be opened, have" << nfiles << "out of" << fileList.count();
    }

    qDebug() << Q_FUNC_INFO << generalInform;
    MI.Option(QStringLiteral("Inform").toStdWString(), generalInform.toStdWString());
    QString informOptionExample=QString::fromStdWString(MI.Inform());
    qDebug() << Q_FUNC_INFO << qPrintable("\r\n\r\nGeneral Inform\r\n") << qPrintable(informOptionExample);

    QStringList informResult=informOptionExample.split('\n',QString::SkipEmptyParts);
    QVariantMap resMap;
    foreach (QString res, informResult) {
        qDebug() << Q_FUNC_INFO << res;
        QStringList resList=res.split("|");
        qDebug() << resList.count() << generalParams.count();
        Q_ASSERT((resList.count()-1)==generalParams.count());
        for (int i=0;i<resList.count()-1;++i) {
            qDebug() << generalParams[i] << ":" << resList[i];
            resMap[generalParams[i]] = resList[i];
        }
        QJsonObject resObject=QJsonObject::fromVariantMap(resMap);
        QString mimeType=resMap["InternetMediaType"].toString();
        if (mimeType.startsWith("audio")) m_audioFiles.append(resObject);
        else if (mimeType.startsWith("video")) m_videoFiles.append(resObject);
        else {
            qWarning() << Q_FUNC_INFO << "mimetype for file" << resMap["CompleteName"]<< "not one of audio or video but" << resMap["InternetMediaType"];
        }
        qDebug() << Q_FUNC_INFO << "resObject" << resObject;
    }
}


void IndexingWorker::indexDirectory(const QString dirPath)
{
    QDir dir(dirPath);
    qDebug() << Q_FUNC_INFO << dir << dirPath;

    // FIXME: this should be configurable and is different on Linux and Mac
    if (dirPath.count("/")>6) return;
    QStringList mediaInfoList;

    dir.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    QStringList dirList=dir.entryList();

    dir.setFilter(QDir::Files);
    dir.setNameFilters(audioFilters);
    QStringList audioList=dir.entryList();
    foreach (QString a, audioList)
        mediaInfoList.append(dirPath+"/"+a);

    dir.setNameFilters(videoFilters);
    QStringList videoList=dir.entryList();
    foreach (QString v, videoList)
        mediaInfoList.append(dirPath+"/"+v);

    mediaInfo(mediaInfoList);

    qDebug() << Q_FUNC_INFO << "appended" <<audioList.count()<<"audio files and"<<videoList.count()<<"video files in"<<dir.absolutePath();
    qDebug() << Q_FUNC_INFO << "moving on to" <<dirList;

    foreach (QString dirName, dirList) {
        // FIXME: take this out and make directory filters configurable
//        if (!dirName.startsWith("rolands"))
            indexDirectory(dirPath+"/"+dirName);

    }
}


FileSystemDevice::FileSystemDevice(QObject *parent)
    : MediaDeviceInterface(parent)
{

}

void FileSystemDevice::updateMediaSourceList(const QUrl url) const
{
    emit mediaSourceListUpdate(getMediaSourceList(url));
}

QJsonObject FileSystemDevice::getMediaSourceList(const QUrl url) const
{
    IndexingWorker worker;
    worker.startIndexing(url);

    // TODO: Define this object somewhere and just fill it here with
    // data.
    // FIXME: If we want C++11 here we have to CONFIG += c++11 not only here
    // but also in MediaInforLib and ZenLib which are statically linked agains this plugin
#if 0
    QJsonObject mediaObject = {
        {"DeviceUrl",url.url()}, // this is likely obsolete
        {"AudioFile",worker.audioFiles()},
        {"VideoFile",worker.videoFiles()}
    };
#endif
    QJsonObject mediaObject;
    mediaObject.insert("DeviceUrl",url.url()); // this is likely obsolete
    mediaObject.insert("AudioFile",worker.audioFiles());
    mediaObject.insert("VideoFile",worker.videoFiles());
    return mediaObject;
}
