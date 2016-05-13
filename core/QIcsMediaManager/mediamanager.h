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
#ifndef MEDIAMANAGER_H
#define MEDIAMANAGER_H

#include <QObject>
#include <QMap>
#include <QMediaPlaylist>

#include "mediasession.h"
#include "mediasource.h"
#include "mediaplayerinterface.h"
#include "mediamanagertypes.h"

class QPluginLoader;
class MediaDeviceFactory;
class MediaManagerControllerInterface;
class DeviceManagerInterface;

/*!
 * \brief The MediaManager class.
 * This class is the central hub of the GENIVI MediaManager.
 * It is meant to manage MediaSource and MediaSession objects,
 * providing necessary function for their playback through the GeniviMediaPlayer
 *
 * When a media device is connected the DeviceManager notifies the MediaManager
 * to create a MediaSource object using the MediaDeviceType if the MediaManager
 * has a plugin loaded that can read the content of the device.
 *
 * The MediaSource object will then notify the MediaManager which playable file
 * types it contains.
 * For each playable file type the MediaManager will instantiate a MediaSession or
 * use an already existing MediaSession.
 * The MediaManager will request a MediaSourcePlaylist for each filetype (e.g. all mp3 files)
 * and hand this list to the corresponding MediaSession.
 *
 * The MediaManager further loads a set of MediaPlayer plugins that implement
 * MediaPlayerInterface. When a MediaSession for a MediaType has been created it receives
 * a plugin that can create player instances for that MediaType.
 *
 * There are two other plugin types that have been implemented. The MediaManagerController
 * is a plugin and its implementations use S&S to control the MediaManager.
 * This way we can have multiple plugins for GUIS and remote control interfaces (RVI or JSonRPC)
 * all loaded and in-sync at the same time.
 *
 * The last interface is the DeviceManager which is also a plugin. Ultimately
 * the plugin can simply launch a thread to monitor DBus or another IPC to figure out that
 * a device has been connected.
 *
 * \ingroup GENIVI_Media_Manager
 */
class MediaManager : public QObject
{
    Q_OBJECT
public:
//    /** Known MediaDevice for which we can load plugins */
//    enum MediaDeviceType { NoDeviceType=0, USBDeviceType, IPodDeviceType };

//    /** Known MediaTypes for which player plugins have been found */
//    enum MediaType {NoMediaType=0, AudioFileMediaType, VideoFileMediaType, EndMediaType };

//    Q_ENUM(MediaDeviceType)
//    Q_ENUM(MediaType)
    typedef mmTypes::MediaDevice MediaDeviceType;
    typedef mmTypes::MediaType MediaType;

    explicit MediaManager(QObject *parent = 0);

    /** This function returns the active Media Session as a QString
     *  Note: Media Sessions are unique pert MediaType. Hence MediaType
     *  is the key used to store the MediaSession pointers. Because
     *  of that one can have a safe QString based interface to access
     *  MediaSessions which in turns is key for the proper functioning
     *  of the Controller plugins.
     **/
    QString activeMediaSession() const;


signals:
    /** Signal emitted when the active MediaSession changes, be it due to a
     *  MediaManagerController actively changing it or by a new device being
     *  detected, inserted or removed. Controller plugins must connect to this
     *  signal to stay in sync and retrieve the MediaPlayerInterface that belongs
     *  to the MediaSession.
     **/
    void activeMediaSessionChanged(const QString mediaType);

    /** Signal emitted when one of the players state changes */
    void mediaPlayStateChanged(MediaPlayerInterface::PlayState state) const;

    /** Signal emitted when the MediaManager wants to notify controllers of the active
     *  MediaSession playlist
     **/
    void activeMediaSessionPlaylist(const QJsonArray playList) const;

    /** Signal emitted when the MediaManager wants to notify controllers of the active
     *  MediaSession current track
     **/
    void currentTrackIndex(const int index) const;

    /** Signal emitted from the MediaManager that requests that Video player plugins
     *  set the video surface playing to a specific rectangle on the screen.
     *  This way controller plugins can position a video surface and create a UI around it.
     */
    void changeVideoRectangle(const QRect rect) const;

    //    void onProgressUpdate(const quint16 progress);
//    void currentSongIndexChanged();
//    void currentlyPlayingChanged(QString currentlyPlaying);
//    void progressChanged(quint16 progress);
//    void currentSongChanged(QString currentTrack);
//    void currentAlbumChanged(QString currentAlbum);
//    void currentArtistChanged(QString currentArtist);
//    void trackIndexChanged(quint16 trackIndex);

public slots:
    /** This slot sets the active MediaSession using the associated MediaType
     *  as a QString argument. See activeMediaSessionType() for further explanation.
     *  There is also a private function that sets the MediaSession by MediaType and
     *  emits a signal.
     **/
    void setActiveMediaSession(const QString mediaSession);

    /** Asks the active MediaSessions MediaPlayer to play.
     *  Fetches the player's status and emits a playStateChanged signal
     **/
    void play() const;
    void pause() const;
    void stop() const;
    void next() const;
    void previous() const;
    //void incrementCurrentVolume() const;
    //void decrementCurrentVolume() const;
    void sendCurrentTrackIndex() const;
    void setCurrentTrackToIndex(int index) const;


protected slots:
    /** Connected to a signal by the DeviceManager plugin and called thereafter when the DeviceManager
     *  notifies that a Device has been connected that is suitable as a MediaSource
     **/
    void createMediaSource(const QString typeStr, const QUrl deviceUrl);
    /** Connected to a signal by the DeviceManager plugin and called when the DeviceManager
     *  notifies that a Device has been removed.
     **/
    void removeMediaSource(const QString typeStr, const QUrl deviceUrl);
    /** Called when a MediaSource has a new or updated MediaSourcePlaylist
     *  Updates all MediaSessions with the updated source lists.
     **/
    void updateMediaSession(const MediaSource * mediaSource);

private:
    /** Load all Plugins */
    void loadPlugins();
    void loadMediaDevicePlugin(QPluginLoader &pluginLoader, const QString pluginName, const int pluginVersionNumber);
    void loadMediaPlayerPlugin(QPluginLoader &pluginLoader, const QString pluginName, const int pluginVersionNumber);
    void loadMediaManagerControllerPlugin(QPluginLoader &pluginLoader, const QString pluginName, const int pluginVersionNumber);
    void loadDeviceManagerPlugin(QPluginLoader &pluginLoader, const QString pluginName, const int pluginVersionNumber);

    /** Helper function that converts a QString to a known MediaType */
//    MediaManager::MediaType mediaTypeFromString(const QString mediaTypeStr) const;
    /** Helper function that converts a MediaType to QString */
//    QString mediaTypeToString(const MediaManager::MediaType type) const;
    /** Helper function that converts a QString to a known MediaDeviceType */
//    MediaManager::MediaDeviceType mediaDeviceTypeFromString(const QString mediaDeviceTypeStr) const;

    /** This is the private implementation, the public one only allows to set the active session by QString.
        It calls this function which emits signal activeMediaSessionChanged */
    void setActiveMediaSession(MediaType mediaType);

    /** There is one active Media Session at all times.
     *  That is the session that all controllers are indirectly working on.
     *  We don't allow any of the plugins to work on pointers or access members directly
     *  but we do allow Controllers to know and store the activeMediaSession.
     **/
    MediaType activeMediaSessionType;

    /** This is currently the only allowed DeviceManagerPlugin
     *  TODO: Make this another QMap and allow to load many device manager plugin types
     **/
    DeviceManagerInterface * deviceManager;


    /** Plugins that implement MediaDeviceFactory.
     *  The plugin implements a factory that creates MediaDevice objects.
     *  MediaDevices produce MediaSourceLists from devices, filesystems, etc.
     *  Each Factory creates objects specific to a device type, e.g.: FileSystemDevice.
     *  MediaManager can instatiate arbitrarily many objects of a MediaDevice.
     */
    QMap<MediaDeviceType,MediaDeviceFactory *> mediaDevicePlugins;

    /** This is a hardcoded list of known media device plugins that
     *  can be loaded and that implement the required interface.
     *  For the time being the key is in the form "PluginName:XYZ"
     *  the expected plugin name and major, minor and bugfix number,
     *  the value is MediaDeviceType.
     */
    QMap<QString,MediaDeviceType> knownMediaDevicePlugins;

    /** MediaSources are created and stored by their Url
     *  TODO: This might have to change to storing the mediaSource
     *  by some other parameter in case two devices show up with the
     *  same URL which might be the case for USB devices.
     *  Maybe we can hash in a USB-ID for the device.
     */
    QMap<QUrl, MediaSource*> mediaSources;
    /** MediaSessions are created and stored by MediaType */
    QMap<MediaType, MediaSession*> mediaSessions;

    /** MediaPlayer plugins that implement MediaPlayerInterface
     *  Currently only one allowed for each MediaType,e.g.
     *  There is only one plugin loaded that can play audio files.
     *  TODO:
     *  This design might have to change since it maybe possible to have
     *  multiple different audio file players running at the same time and playing
     *  independently to different sinks. However if the plugin
     *  can create multiple instances of an actual player then this
     *  is a sufficient architecture
     */
    QMap<MediaType,MediaPlayerInterface *> mediaPlayerPlugins;

    /** Hardcoded list of known media player plugins that
     *  can be loaded and that provide a required interface
     *  to play media of a MediaType.
     *  As with the MediaDevicePlugins the key is again in the form
     *  PluginName:XYZ whereas the value is a MediaType
     *  used to store the actual Plugin.
     */
    QMap<QString,MediaType> knownMediaPlayerPlugins;

    /** MediaManagerControllerPlugins.
     *  There can be several plugins that control the state of the MediaManager.
     *  This will only work if the Controllers are stateless.
     **/
    QMap<QString,MediaManagerControllerInterface *> controllerPlugins;

    /** Hardcoded list of known controller plugins.
     *  Controllers, well, they control the MediaManager,
     *  i.e. make it play, pause, stop, etc..
     **/
    QMap<QString,QString> knownControllerPlugins;

    /** Hardcoded list of known device manager plugins.
     *  These are supposed to figure out when a new device
     *  was connected and notify the MediaManager of it.
     **/
    QMap<QString,QString> knownDeviceManagerPlugins;

};

#endif // MEDIAMANAGER_H
