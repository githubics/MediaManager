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
#ifndef SIMPLEUICONTROLLER_H
#define SIMPLEUICONTROLLER_H

#include <QQmlEngine>
#include <QQmlContext>
#include <QtQml>
#include <QQuickView>

#include <QtQuick/QQuickView>

#include "../QIcsMediaManager/mediamanagercontrollerinterface.h"
#include "../QIcsMediaManager/mediaplayerinterface.h"
class PlayListModel;

class SimpleUiController : public MediaManagerControllerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ics.media-manager.MediaManagerControllerInterface" FILE "SimpleUiController.json")
    Q_INTERFACES(MediaManagerControllerInterface)
    Q_PROPERTY(QJsonObject currentTrack READ currentTrack NOTIFY currentTrackChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentTrackIndex NOTIFY currentTrackChanged)
    Q_PROPERTY(QString activeMediaSession READ activeMediaSession  WRITE requestActiveMediaSession  NOTIFY activeMediaSessionChanged)
    Q_PROPERTY(QVariantList audioPlaylist READ audioPlaylist NOTIFY audioPlaylistChanged)
    Q_PROPERTY(QVariantList videoPlaylist READ videoPlaylist NOTIFY videoPlaylistChanged)
    Q_PROPERTY(QStringList mediaSessions READ mediaSessions WRITE setMediaSessions NOTIFY mediaSessionsChanged)
    Q_PROPERTY(MediaPlayerInterface::PlayState playState READ playState WRITE setPlayState NOTIFY playStateChanged)

public:
    Q_ENUMS(MediaPlayerInterface::PlayState)
    SimpleUiController();
    ~SimpleUiController() {}

    QString activeMediaSession() const;

    /** UI can request to set an active media session */
    Q_INVOKABLE void requestActiveMediaSession(const QString activeSession);

    /** Called by the UI */
    Q_INVOKABLE void play() const;
    Q_INVOKABLE void pause() const;
    Q_INVOKABLE void stop() const;
    Q_INVOKABLE void previous() const;
    Q_INVOKABLE void next() const;
    // TODO: See the comment in the corresponding qml file that calls this function
    Q_INVOKABLE void setVideoRectangle(int x, int y, int height, int width);

    QStringList mediaSessions() const;
    QVariantList audioPlaylist() const;
    QVariantList videoPlaylist() const;
    int currentIndex() const;
    QJsonObject currentTrack() const;
    MediaPlayerInterface::PlayState playState() const;


public slots:


    /** From the MediaManager to Controller*/
    void setActiveMediaSession(QString activeMediaSession);
    void setActiveMediaSessionPlaylist(const QJsonArray playList);
    void setMediaSessions(const QStringList mediaSessions);
    void setPlayState(const MediaPlayerInterface::PlayState playState);
    void setCurrentTrackIndex(const int index);

signals:
    /** From this object to the UI */
    void activeMediaSessionChanged(QString activeMediaSession);
    void mediaSessionsChanged();
    void audioPlaylistChanged();
    void videoPlaylistChanged();
    void currentTrackChanged();
    void playStateChanged();


private:

    QQmlEngine * engine;
    QUrl qml();
    QQuickView * quickView;
    QString m_activeMediaSession;
    QStringList m_mediaSessions;
    PlayListModel * myAudioModel;
    PlayListModel * myVideoModel;
    MediaPlayerInterface::PlayState myState;
};

#endif // SIMPLEUICONTROLLER_H
