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
#include <QStringList>
#include <QDir>
#include <QJsonObject>
#include <QTime>
#include <QTimer>
#include <QCoreApplication>
#include <QPluginLoader>
#include <QJsonDocument>
#include <QJsonArray>
#include <QRect>

#include "mediadevicefactory.h"
#include "mediadeviceinterface.h"
#include "mediamanagercontrollerinterface.h"
#include "mediaplayerinterface.h"
#include "devicemanagerinterface.h"

#include "mediasource.h"

#ifdef Q_OS_WIN
static const QString LibPrefix = "";
static const QString LibExtension = "dll";
#elif defined Q_OS_MACX
static const QString LibExtension = "dylib";
static const QString LibPrefix = "lib";
#else
static const QString LibExtension = "so";
static const QString LibPrefix = "lib";
#endif


#include "mediamanager.h"


MediaManager::MediaManager(QObject *parent)
    : QObject(parent)
    , activeMediaSessionType(mmTypes::NoType)
    , deviceManager(0)
{
    // This is a hardcoded list of media device plugins and minimum required version numbers
    knownMediaDevicePlugins["FileSystemDevice:120"] = mmTypes::USBDevice ;
    knownMediaDevicePlugins["IPodDevice:100"] = mmTypes::IPodDevice ;

    // This is a hardcoded list of media player plugins and minimum required version numbers
    knownMediaPlayerPlugins["QIcsMediaPlayer:100"] = mmTypes::AudioFile;
    knownMediaPlayerPlugins["QIcsAudioVideoPlayer:120"] = mmTypes::VideoFile;

    // Hardcoded list of controller plugins.
    knownControllerPlugins["SimpleTcpController:100"] = "ReadWriteContollerType";
    knownControllerPlugins["TcpJsonRpcController:100"] = "ReadWriteContollerType";
    knownControllerPlugins["SimpleUiController:100"] = "ReadWriteContollerType";
    knownControllerPlugins["HttpJsonRpcController:100"] = "ReadWriteContollerType";

    // Hardcoded list of device manager plugins.
    knownDeviceManagerPlugins["SimpleDeviceManager:100"] = "MountedDeviceManager";

    loadPlugins();

}

void MediaManager::play() const
{
    if (activeMediaSessionType==mmTypes::NoType) return;
    mediaSessions[activeMediaSessionType]->player()->play();
    emit currentTrackIndex(mediaSessions[activeMediaSessionType]->player()->currentTrackIndex());
}
void MediaManager::pause() const
{
    if (activeMediaSessionType==mmTypes::NoType) return;
    mediaSessions[activeMediaSessionType]->player()->pause();
    emit currentTrackIndex(mediaSessions[activeMediaSessionType]->player()->currentTrackIndex());
}
void MediaManager::stop() const
{
    if (activeMediaSessionType==mmTypes::NoType) return;
    mediaSessions[activeMediaSessionType]->player()->stop();
    emit currentTrackIndex(mediaSessions[activeMediaSessionType]->player()->currentTrackIndex());
}
void MediaManager::next() const
{
    if (activeMediaSessionType==mmTypes::NoType) return;
    mediaSessions[activeMediaSessionType]->player()->next();
}
void MediaManager::previous() const
{
    if (activeMediaSessionType==mmTypes::NoType) return;
    mediaSessions[activeMediaSessionType]->player()->previous();
}

void MediaManager::sendCurrentTrackIndex() const
{
    if (activeMediaSessionType==mmTypes::NoType) return;
    emit currentTrackIndex(mediaSessions[activeMediaSessionType]->player()->currentTrackIndex());
}

void MediaManager::setCurrentTrackToIndex(int index) const
{
    if (activeMediaSessionType==mmTypes::NoType) return;
       mediaSessions[activeMediaSessionType]->player()->setCurrentTrack(index);
}

QString MediaManager::activeMediaSession() const
{
    return mmTypes::Media(activeMediaSessionType);
}

void MediaManager::setActiveMediaSession(const QString mediaSession)
{

    MediaType mst=(MediaType)mmTypes::Media(mediaSession);
    setActiveMediaSession(mst);

}

void MediaManager::setActiveMediaSession(MediaManager::MediaType mediaType)
{
    if (activeMediaSessionType!=mmTypes::NoType) {
        const MediaSession * ms=mediaSessions[activeMediaSessionType];
        const MediaPlayerInterface * mp=ms->player();
        disconnect(mp,&MediaPlayerInterface::playStateChanged,this,&MediaManager::mediaPlayStateChanged);
    }

    if (!mediaSessions.contains(mediaType)) return;
    if (mediaType>mmTypes::NoType)
        activeMediaSessionType=mediaType;
    emit activeMediaSessionChanged(mmTypes::Media(mediaType));

    emit activeMediaSessionPlaylist(mediaSessions[activeMediaSessionType]->mediaSessionPlaylist());

    if (activeMediaSessionType!=mmTypes::NoType) {
        const MediaSession * ms=mediaSessions[activeMediaSessionType];
        const MediaPlayerInterface * mp=ms->player();
        connect(mp,&MediaPlayerInterface::playStateChanged,this,&MediaManager::mediaPlayStateChanged);
    }
}

void MediaManager::createMediaSource(const QString typeStr, const QUrl deviceUrl)
{
//    const MediaManager::MediaDeviceType type=mediaDeviceTypeFromString(typeStr);
    const MediaDeviceType type=(MediaDeviceType)mmTypes::Device(typeStr);
    if (type==mmTypes::NoDevice) {
        qWarning() << Q_FUNC_INFO << "Unknown MediaDeviceType, no plugins known that can index devices of type" << typeStr;
        return;
    }

    qDebug() << Q_FUNC_INFO << type << deviceUrl;
    if (!mediaDevicePlugins.contains(type)) {
        qWarning() << Q_FUNC_INFO << "No plugin to read device of type" << type <<"could be loaded";
        return;
    }

    /* Load the MediaSource object and hand the plugin to it.
     * Then connect signals from it to notify when a MediaSourcePlaylist is ready
     * for the MediaManager to split up and hand things to to a MediaSession object
     */
    if (!mediaSources.contains(deviceUrl)) {
        MediaDeviceFactory * msi=mediaDevicePlugins[type];
        Q_ASSERT(msi!=0);
        MediaDeviceInterface * mdi=msi->createMediaDevice(this);
        MediaSource * ms=new MediaSource(mdi,deviceUrl, this);
        connect (ms, &MediaSource::mediaSourcePlaylistChanged, this, &MediaManager::updateMediaSession);
        mediaSources.insert(deviceUrl,ms);
        ms->updateMediaSourcePlaylist();
    }
}

void MediaManager::removeMediaSource(const QString typeStr, const QUrl deviceUrl)
{
    const MediaDeviceType type=(MediaDeviceType)mmTypes::Device(typeStr);
    if (type==mmTypes::NoDevice) {
        qWarning() << Q_FUNC_INFO << "Unknown MediaDeviceType, no plugins known that can index devices of type" << typeStr;
        return;
    }
    qDebug() << Q_FUNC_INFO << type << deviceUrl;
    Q_ASSERT(mediaSources.contains(deviceUrl)); // can not remove what wasn't inserted in the first place!
    MediaSource * ms=mediaSources.take(deviceUrl);

    // FIXME: This would have been easier if we just looped over all sessions and
    // removed the deviceUrlStr related data.
    const QString deviceUrlStr=ms->deviceUrlString();
    for (int mt=mmTypes::NoType+1;mt<mmTypes::EndType;++mt)
    {
        MediaType mediaType=(const MediaType)mt;
        QString mediaTypeStr=mmTypes::Media(mediaType);
        if (!ms->hasMediaType(mediaTypeStr)) continue;
        if (mediaSessions.contains(mediaType)) {
            mediaSessions[mediaType]->removeMediaSourcePlaylist(deviceUrlStr);          
        }
    }
    emit activeMediaSessionPlaylist(mediaSessions[activeMediaSessionType]->mediaSessionPlaylist());
    ms->deleteLater();
}

void MediaManager::updateMediaSession(const MediaSource * mediaSource)
{
//    qDebug() << Q_FUNC_INFO << mediaSource;
    const QString deviceUrlStr=mediaSource->deviceUrlString();

    for (int mt=mmTypes::NoType+1;mt<mmTypes::EndType;++mt)
    {
        MediaType mediaType=(const MediaType)mt;
        QString mediaTypeStr=mmTypes::Media(mediaType);
        qDebug() << Q_FUNC_INFO << "looking for JSonArray for MediaType" << mediaTypeStr << "DeviceUrl" << deviceUrlStr;

        const QJsonArray playListArray=mediaSource->mediaArray(mediaTypeStr);
        if (playListArray.isEmpty()) {
            qDebug() << Q_FUNC_INFO << "no media of type" << mediaTypeStr << "found in" << deviceUrlStr;
            continue;
        }

        MediaSession * mediaSession=0;
        if (mediaSessions.contains(mediaType)) {
            mediaSession=mediaSessions[mediaType];
        }
        else {
            MediaPlayerInterface * player=0;
            if (mediaPlayerPlugins.contains(mediaType)) {
                player=mediaPlayerPlugins[mediaType];
            }
            else {
                qWarning() << Q_FUNC_INFO << "no plugin found for MediaType" << mediaType;
                continue;
            }
            mediaSession=new MediaSession(player, this);
            mediaSessions.insert(mediaType, mediaSession);
        }
        mediaSession->appendMediaSourcePlaylist(deviceUrlStr,playListArray);
        // FIXME: we need some smart resolution when inserting a media changes the active
        // media session, it may or may not be desirable to do that. Currently we just leave
        // the first one active until the user changes it through the controller
        if (mediaSessions.count()==1)
            setActiveMediaSession(mediaType);
        else
            emit activeMediaSessionPlaylist(mediaSessions[activeMediaSessionType]->mediaSessionPlaylist());
    }
}


// TODO: Create a MediaManagerPluginLoader class that can get rid of the
// code duplication here and load the plugins as required
// This might be a stretch goal
void MediaManager::loadPlugins()
{
    QTime pluginLoadTime;
    pluginLoadTime.start();

    QDir pluginsDir = QDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cdUp();
    pluginsDir.cd("plugins");
    qDebug() << Q_FUNC_INFO << "Loading plugins from:" << pluginsDir.path();

    QStringList pluginFilter;
    pluginFilter << QString("*.%1").arg(LibExtension);
    QStringList pluginsList = pluginsDir.entryList(pluginFilter);
    foreach (QString candidate, pluginsList) {
        QString fileName = QString("%1").arg(pluginsDir.absoluteFilePath(candidate));
        qDebug() << Q_FUNC_INFO << "Loading" << fileName;
        QPluginLoader pluginLoader( fileName );
        QJsonObject metaData = pluginLoader.metaData();
        qDebug() << Q_FUNC_INFO << "MetaData" << metaData;

        // There will be four different plugin types but currently we have only two
        QString pluginTypeString;
        if (metaData.value("IID").toString() == MediaDeviceFactory_iid) pluginTypeString="DevicePlugin";
        else if (metaData.value("IID").toString() == MediaPlayerInterface_iid) pluginTypeString="MediaPlayerPlugin";
        else if (metaData.value("IID").toString() == DeviceManagerInterface_iid) pluginTypeString="DeviceManagerPlugin";
        else if (metaData.value("IID").toString() == MediaManagerControllerInterface_iid) pluginTypeString="MediaManagerControllerPlugin";
        if (pluginTypeString.isNull()) {
            qDebug() << Q_FUNC_INFO << "Wrong MetaData IID, found" << metaData.value("IID").toString() << "expected one of\n"
                     << MediaDeviceFactory_iid
                     << MediaPlayerInterface_iid
                     << DeviceManagerInterface_iid
                     << MediaManagerControllerInterface_iid ;
            continue;
        }

        metaData = metaData.value("MetaData").toObject();
        QString pluginName = metaData.value("name").toString();
        if (pluginName.isEmpty()) {
            qDebug() << Q_FUNC_INFO << "Plugin does not provide name in metadata; file:" << fileName;
            continue;
        }
        QString pluginVersion = metaData.value("version").toString();
        if (pluginVersion.isEmpty()) {
            qDebug() << Q_FUNC_INFO << "Plugin does not provide version in metadata; file:" << fileName;
            continue;
        }
        int pluginVersionNumber=pluginVersion.replace(".","").toInt();
        qDebug() << Q_FUNC_INFO << pluginName << pluginVersionNumber << fileName;

        if (pluginTypeString=="DevicePlugin")
            loadMediaDevicePlugin(pluginLoader, pluginName, pluginVersionNumber);
        else if (pluginTypeString=="MediaPlayerPlugin")
            loadMediaPlayerPlugin(pluginLoader, pluginName, pluginVersionNumber);
        else if (pluginTypeString=="DeviceManagerPlugin")
            loadDeviceManagerPlugin(pluginLoader, pluginName, pluginVersionNumber);
        else if (pluginTypeString=="MediaManagerControllerPlugin")
            loadMediaManagerControllerPlugin(pluginLoader, pluginName, pluginVersionNumber);
    }
    int nPlugins=mediaDevicePlugins.count() + mediaPlayerPlugins.count()+controllerPlugins.count()+(deviceManager!=0?1:0);
    qDebug() << Q_FUNC_INFO << "Finished loading:" << nPlugins << "plugins in" << pluginLoadTime.elapsed() << "ms";

}

// I really hate the cut and paste job and the senseless code duplication here but I also don't want to overengineer this code.
// and actually I can't think of a better way to do this.
void MediaManager::loadMediaPlayerPlugin(QPluginLoader & pluginLoader, const QString pluginName, const int pluginVersionNumber)
{
    bool loadPlugin=false;

    QString pluginMediaTypes = pluginLoader.metaData().value("MetaData").toObject().value("media").toString();
    qDebug() << Q_FUNC_INFO << pluginMediaTypes;
    // TODO: Check that sufficient file types are supported.

    MediaType pluginType=mmTypes::NoType;
    foreach (QString s, knownMediaPlayerPlugins.keys()) {
        QStringList sl=s.split(":");
        if (sl[0] == pluginName) {
            int minVersion=sl[1].toInt();
            if (pluginVersionNumber>=minVersion) {
                pluginType=knownMediaPlayerPlugins[s];
                loadPlugin=true;
                break;
            }
            else {
                qDebug() << Q_FUNC_INFO << "Plugin version to old. Found plugin in list of knowMediaPlayerPlugins"
                         << "minimum Version required" << minVersion << "found" << pluginVersionNumber;
                continue;
            }
        }
    }
    if (!loadPlugin) {
        qDebug() << Q_FUNC_INFO << "Plugin not loaded:" << pluginName << pluginVersionNumber
                 << "Not found in list of valid plugins.";
        return;
    }

    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        MediaPlayerInterface * mplPlugin = qobject_cast<MediaPlayerInterface*>(plugin);
        if (!mplPlugin) {
            qWarning() << Q_FUNC_INFO << "Plugin not loaded. Name and version okay but plugin did not load.";
        }
        else {
            if (!mediaPlayerPlugins.contains(pluginType)) {
                qDebug() << Q_FUNC_INFO << "inserting plugin of MediaType" << pluginType << "with pointer" << mplPlugin;
                mediaPlayerPlugins[pluginType]=mplPlugin;
                connect(mplPlugin, &MediaPlayerInterface::currentTrackIndexChanged, this, &MediaManager::currentTrackIndex);

                connect(this,&MediaManager::changeVideoRectangle,mplPlugin,&MediaPlayerInterface::setVideoRectangle);
            }
            else {
                qWarning() << Q_FUNC_INFO << "Multiple plugins with the same name found. First one will be used.";
                pluginLoader.unload();
            }
        }
    } else {
        qDebug() << Q_FUNC_INFO << "Plugin Load Error:";
        qDebug() << pluginLoader.errorString();
    }

}

void MediaManager::loadMediaDevicePlugin(QPluginLoader & pluginLoader, const QString pluginName, const int pluginVersionNumber)
{
    bool loadPlugin=false;
    MediaDeviceType pluginType=mmTypes::NoDevice;
    foreach (QString s, knownMediaDevicePlugins.keys()) {
        QStringList sl=s.split(":");
        if (sl[0] == pluginName) {
            int minVersion=sl[1].toInt();
            if (pluginVersionNumber>=minVersion) {
                pluginType=knownMediaDevicePlugins[s];
                loadPlugin=true;
                break;
            }
            else {
                qDebug() << Q_FUNC_INFO << "Plugin version to old. Found plugin in list of knowMediaDevicePlugins"
                         << "minimum Version required" << minVersion << "found" << pluginVersionNumber;
                continue;

            }
        }
    }
    if (!loadPlugin) {
        qDebug() << Q_FUNC_INFO << "Plugin not loaded:" << pluginName << pluginVersionNumber
                 << "Not found in list of valid plugins.";
        return;
    }

    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        MediaDeviceFactory * mdiPlugin = qobject_cast<MediaDeviceFactory*>(plugin);
        if (!mdiPlugin) {
            qWarning() << Q_FUNC_INFO << "Plugin not loaded. Name and version okay but plugin did not load.";
        }
        else {
            if (!mediaDevicePlugins.contains(pluginType)) {
                qDebug() << Q_FUNC_INFO << "inserting plugin of MediaType" << pluginType << "with pointer" << mdiPlugin;
                mediaDevicePlugins[pluginType]=mdiPlugin;
            }
            else {
                qWarning() << Q_FUNC_INFO << "Multiple plugins with the same name found. First one will be used.";
                pluginLoader.unload();
            }
        }
    } else {
        qDebug() << Q_FUNC_INFO << "Plugin Load Error:";
        qDebug() << pluginLoader.errorString();
    }

}

void MediaManager::loadMediaManagerControllerPlugin(QPluginLoader & pluginLoader, const QString pluginName, const int pluginVersionNumber)
{
    qDebug() << Q_FUNC_INFO;
    bool loadPlugin=false;

    QString ctrlTypeStr;
    foreach (QString s, knownControllerPlugins.keys()) {
        QStringList sl=s.split(":");
        if (sl[0] == pluginName) {
            int minVersion=sl[1].toInt();
            if (pluginVersionNumber>=minVersion) {
                ctrlTypeStr=knownControllerPlugins[s];
                loadPlugin=true;
                break;
            }
            else {
                qDebug() << Q_FUNC_INFO << "Plugin version to old. Found plugin in list of knownControllerPlugins"
                         << "minimum Version required" << minVersion << "found" << pluginVersionNumber;
                continue;
            }
        }
    }

    if (!loadPlugin) {
        qDebug() << Q_FUNC_INFO << "Plugin not loaded:" << pluginName << pluginVersionNumber
                 << "Not found in list of valid plugins.";
        return;
    }

    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        MediaManagerControllerInterface * ctrlPlugin = qobject_cast<MediaManagerControllerInterface*>(plugin);
        if (!ctrlPlugin) {
            qWarning() << Q_FUNC_INFO << "Plugin not loaded. Name and version okay but plugin did not load.";
        }
        else {
            if (controllerPlugins.contains(ctrlTypeStr)) {
                // TODO: this is a little crude and could be improved, in fact we might want to change this to a MultiMap
                ctrlTypeStr+="_1";
                qDebug() << Q_FUNC_INFO << "Multiple plugins of the same type found. All will be loaded.";
            }
            qDebug() << Q_FUNC_INFO << "inserting plugin of ControllerType" << ctrlTypeStr << "with pointer" << ctrlPlugin;
            controllerPlugins[ctrlTypeStr]=ctrlPlugin;
            // From the MediaManager to the Controller
            connect(this, &MediaManager::activeMediaSessionChanged,ctrlPlugin, &MediaManagerControllerInterface::setActiveMediaSession);
            connect(this, &MediaManager::activeMediaSessionPlaylist,ctrlPlugin, &MediaManagerControllerInterface::setActiveMediaSessionPlaylist);
            connect(this, &MediaManager::currentTrackIndex,ctrlPlugin, &MediaManagerControllerInterface::setCurrentTrackIndex);

            connect(this, &MediaManager::mediaPlayStateChanged,ctrlPlugin, &MediaManagerControllerInterface::setPlayState);

            // From the Controller to the MediaManager
            connect(ctrlPlugin, &MediaManagerControllerInterface::requestSetActiveMediaSessionTo, this,
                    static_cast < void(MediaManager::*)(const QString) > (&MediaManager::setActiveMediaSession));

            connect(ctrlPlugin, &MediaManagerControllerInterface::requestCurrentTrackIndex, this, &MediaManager::sendCurrentTrackIndex);
            connect(ctrlPlugin, &MediaManagerControllerInterface::requestTrackAtIndex, this, &MediaManager::setCurrentTrackToIndex);

            connect(ctrlPlugin, &MediaManagerControllerInterface::requestPlay, this, &MediaManager::play);
            connect(ctrlPlugin, &MediaManagerControllerInterface::requestPause, this, &MediaManager::pause);
            connect(ctrlPlugin, &MediaManagerControllerInterface::requestStop, this, &MediaManager::stop);
            connect(ctrlPlugin, &MediaManagerControllerInterface::requestNext, this, &MediaManager::next);
            connect(ctrlPlugin, &MediaManagerControllerInterface::requestPrevious, this, &MediaManager::previous);
            connect(ctrlPlugin, &MediaManagerControllerInterface::requestChangeVideoRectangle, this, &MediaManager::changeVideoRectangle);
        }
    } else {
        qDebug() << Q_FUNC_INFO << "Plugin Load Error:";
        qDebug() << pluginLoader.errorString();
    }
}

void MediaManager::loadDeviceManagerPlugin(QPluginLoader & pluginLoader, const QString pluginName, const int pluginVersionNumber)
{
    bool loadPlugin=false;
    QString pluginType;
    foreach (QString s, knownDeviceManagerPlugins.keys()) {
        QStringList sl=s.split(":");
        if (sl[0] == pluginName) {
            int minVersion=sl[1].toInt();
            if (pluginVersionNumber>=minVersion) {
                pluginType=knownDeviceManagerPlugins[s];
                loadPlugin=true;
                break;
            }
            else {
                qDebug() << Q_FUNC_INFO << "Plugin version to old. Found plugin in list of knowMediaDevicePlugins"
                         << "minimum Version required" << minVersion << "found" << pluginVersionNumber;
                continue;

            }
        }
    }
    if (!loadPlugin) {
        qDebug() << Q_FUNC_INFO << "Plugin not loaded:" << pluginName << pluginVersionNumber
                 << "Not found in list of valid plugins.";
        return;
    }

    QObject *plugin = pluginLoader.instance();
    if (plugin) {
        DeviceManagerInterface * dmPlugin = qobject_cast<DeviceManagerInterface*>(plugin);
        if (!dmPlugin) {
            qWarning() << Q_FUNC_INFO << "Plugin not loaded. Name and version okay but plugin did not load.";
        }
        else {
            if (!deviceManager) {
                qDebug() << Q_FUNC_INFO << "inserting plugin of type" << pluginType << "with pointer" << dmPlugin;
                deviceManager=dmPlugin;
                // This is where connections between MediaManager and DeviceManager go
                connect(deviceManager, &DeviceManagerInterface::deviceCreated, this, &MediaManager::createMediaSource);
                connect(deviceManager, &DeviceManagerInterface::deviceRemoved, this, &MediaManager::removeMediaSource);
            }
            else {
                qWarning() << Q_FUNC_INFO << "Multiple plugins with the same name found. First one will be used.";
                pluginLoader.unload();
            }
        }
    } else {
        qDebug() << Q_FUNC_INFO << "Plugin Load Error:";
        qDebug() << pluginLoader.errorString();
    }
}
