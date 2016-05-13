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
#ifndef MEDIAMANAGERCONTROLLERINTERFACE
#define MEDIAMANAGERCONTROLLERINTERFACE
#include <QtPlugin>

// TODO when the enums are moved to their own file this is no longer needed
#include "mediaplayerinterface.h"

#include <QObject>
#include <QJsonArray>
#include <QStringList>

/*!
 * \brief MediaManagerControllerInterface
 *
 * This interface is implemented by MediaManagerController plugins.
 *
 * \ingroup GENIVI_Media_Manager
 */

// forward declare Qt classes
QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE


class MediaManagerControllerInterface : public QObject
{
    Q_OBJECT
public:

    explicit MediaManagerControllerInterface(QObject * parent = 0) : QObject(parent) {}
    virtual ~MediaManagerControllerInterface() {}

public slots:
    /** The slot is connected to a signal of the MediaManager when the active MediaSession changes.
     **/
    virtual void setActiveMediaSession(const QString) = 0;
    virtual void setActiveMediaSessionPlaylist(const QJsonArray) = 0;
    virtual void setMediaSessions(const QStringList) = 0;
    virtual void setPlayState(const MediaPlayerInterface::PlayState) = 0;
    virtual void setCurrentTrackIndex(const int) = 0;

signals:
    /** From this Controller to the MediaManager.
     *  MediaController requests an action from the MediaManager and in turn
     *  receives a signal connected to a slot that receives an answer
     **/
    void requestSetActiveMediaSessionTo(const QString); // answered by setActiveMediaSession() and setActiveMediaSessionPlaylist()
    void requestMediaSessions();        // TODO: not yet emitted, not yet answered
    void requestCurrentTrackIndex();    // answered by setCurrentTrackIndex(const int index)
    void requestPlay() const;           // answered by setPlayState()
    void requestPause() const;          // answered by setPlayState()
    void requestStop() const;           // answered by setPlayState()
    void requestNext() const;           // answered by setPlayState()
    void requestPrevious() const;       // answered by setPlayState()
    void requestTrackAtIndex(int index);
    /** This request is emitted from the controller to try and have the media player set a
     *  video surface rectangle. The argument is likely going to change since we want to
     *  have other properties of the video players change too.
     *  The MediaManager will answer the request with a setVideoRectangle()
     */
    void requestChangeVideoRectangle(QRect viewRect) const;
    //void incrementCurrentVolume();
    //void decrementCurrentVolume();
};

QT_BEGIN_NAMESPACE

#define MediaManagerControllerInterface_iid "com.ics.media-manager.MediaManagerControllerInterface"

Q_DECLARE_INTERFACE(MediaManagerControllerInterface, MediaManagerControllerInterface_iid)

QT_END_NAMESPACE



#endif // MEDIAMANAGERCONTROLLERINTERFACE

