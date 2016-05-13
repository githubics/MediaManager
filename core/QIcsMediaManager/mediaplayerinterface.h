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
#ifndef MEDIAPLAYERINTERFACE_H
#define MEDIAPLAYERINTERFACE_H
#include <QtPlugin>

#include <QObject>
#include <QRect>

// forward declare Qt classes
QT_BEGIN_NAMESPACE
class QStringList;
QT_END_NAMESPACE

class MediaPlayerInterface : public QObject
{
    Q_OBJECT

public:
    enum PlayState {Playing = 0, Paused, Stopped};
    Q_ENUM(PlayState)

    explicit MediaPlayerInterface(QObject * parent=0) : QObject(parent) {}
    virtual ~MediaPlayerInterface() {}
    virtual void setMediaPlaylist(const QStringList playList) = 0;
//    virtual void addMediaPlaylist(const QStringList playList) = 0;
    virtual const QSet<QString> supportedFileSuffixes() const = 0;
    virtual PlayState playState() const = 0;
    /** Returns the index of the curren track in the players playlist */
    virtual int currentTrackIndex() const = 0;

public slots:
    virtual void play() const = 0;
    virtual void pause() const = 0;
    virtual void stop() const = 0;
    virtual void next() const = 0;
    virtual void previous() const = 0;
    virtual void setCurrentTrack(int index) const = 0;
    virtual void setVideoRectangle(const QRect rect) const {Q_UNUSED(rect)};

signals:
    /** Must emit this signal so that the Controllers are notified
     *  when the current state changes
     **/
    void playStateChanged(PlayState state);
    /** Must emit this signal so that the Controllers are notified
     *  when the current track changes
     **/
    void currentTrackIndexChanged(int index);

//    void currentTrackChanged(QString track);
//    void progressChanged(int progress);
};

QT_BEGIN_NAMESPACE

#define MediaPlayerInterface_iid "com.ics.media-manager.MediaPlayerInterface"

Q_DECLARE_INTERFACE(MediaPlayerInterface, MediaPlayerInterface_iid)

QT_END_NAMESPACE
#endif // MEDIAPLAYERINTERFACE_H

