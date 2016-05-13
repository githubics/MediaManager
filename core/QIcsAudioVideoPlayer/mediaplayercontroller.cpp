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
#include <QJsonDocument>
#include "mediaplayercontroller.h"

MediaPlayerController::MediaPlayerController(QObject *parent)
    : QObject(parent)
    , m_playbackState(MediaPlayerController::StoppedState)
    , m_autoAdvance(true)
    , m_loopAtEnd(true)
    , m_switchingTracks(false)
    , m_trackIndex(-1)
    , m_mediaMode("video")
{
}

QString MediaPlayerController::track() const
{
    return m_track;
}

void MediaPlayerController::setTrack(QString track)
{
    if (m_track == track)
        return;
    m_track = track;
    emit trackChanged(track);
}

MediaPlayerController::PlaybackState MediaPlayerController::playbackState() const
{
    return m_playbackState;
}

void MediaPlayerController::playTrack()
{
    emit play();
}

void MediaPlayerController::pauseTrack()
{
    emit pause();
}

void MediaPlayerController::stopTrack()
{
    bool autoAdvanceState=m_autoAdvance;
    m_autoAdvance=false; // stop means stop, no more auto advancing.
    emit stop();
    m_autoAdvance=autoAdvanceState;
}

QStringList MediaPlayerController::audioTracks() const
{
    return m_audioTracks;
}

void MediaPlayerController::setAudioTracks(const QStringList &audioTracks)
{
    m_audioTracks = audioTracks;
    qDebug() << Q_FUNC_INFO << "received play list" << m_audioTracks;
    if (m_audioTracks.count()>0) {
        m_trackIndex=0;
        // TODO: make that configurable
        // if we wanted to automagically play when a playlist is changed then we can uncomment the next line
//        setTrackByIndex(0);
    }
}

QStringList MediaPlayerController::videoTracks() const
{
    return m_videoTracks;
}

void MediaPlayerController::setVideoTracks(const QStringList &videoTracks)
{
    m_videoTracks = videoTracks;
    qDebug() << Q_FUNC_INFO << "received play list" << m_videoTracks;
    if (m_videoTracks.count()>0) {
        m_trackIndex=0;
        // TODO: make that configurable
        // if we wanted to automagically play when a playlist is changed then we can uncomment the next line
        setTrackByIndex(0);
    }
}


void MediaPlayerController::setMediaMode(QString mediaMode)
{
    if (QString()==mediaMode) return;
    if ((mediaMode!="audio")&&(mediaMode!="video")) return;
    if (m_mediaMode == mediaMode) return;
    m_mediaMode = mediaMode;
    emit mediaModeChanged(mediaMode);
}

void MediaPlayerController::setTrackTitle(QString trackTitle)
{
    if (m_trackTitle == trackTitle)
        return;

    m_trackTitle = trackTitle;
    emit trackTitleChanged(trackTitle);
}

void MediaPlayerController::setTrackPerformer(QString trackPerformer)
{
    if (m_trackPerformer == trackPerformer)
        return;

    m_trackPerformer = trackPerformer;
    emit trackPerformerChanged(trackPerformer);
}

void MediaPlayerController::setTrackAlbum(QString trackAlbum)
{
    if (m_trackAlbum == trackAlbum)
        return;

    m_trackAlbum = trackAlbum;
    emit trackAlbumChanged(trackAlbum);
}

void MediaPlayerController::setPlaybackState(PlaybackState playbackState)
{
    qDebug() << Q_FUNC_INFO << "now" << playbackState << "was" <<m_playbackState;
    if (m_playbackState == playbackState)
        return;

    const QStringList tracks = (m_mediaMode=="video")?m_videoTracks:m_audioTracks;

    m_playbackState = playbackState;

    if (m_playbackState==StoppedState) {    // if we are stopped
        if (!m_switchingTracks) {           // and if we are not currently switching tracks already (this is important because QML MediaPlayer send stopped in between songs)
            emit playbackStateChanged(m_playbackState);
            if (m_autoAdvance) {            // and if we are auto advancing
                int idx=m_trackIndex+1;     // and if we are not at the end of the playlist and looping at the end
                if ((idx==tracks.count()&&m_loopAtEnd)) idx=0;
                if ((idx<tracks.count())) {
                    setTrackByIndex(idx);
                    playTrack();
                }
            }
        }
    }
    else {
        emit playbackStateChanged(m_playbackState);
    }
}

QString MediaPlayerController::mediaMode() const
{
    return m_mediaMode;
}

QString MediaPlayerController::trackTitle() const
{
    return m_trackTitle;
}

QString MediaPlayerController::trackPerformer() const
{
    return m_trackPerformer;
}

QString MediaPlayerController::trackAlbum() const
{
    return m_trackAlbum;
}

int MediaPlayerController::trackIndex() const
{
    return m_trackIndex;
}

bool MediaPlayerController::autoAdvance() const
{
    return m_autoAdvance;
}

void MediaPlayerController::setAutoAdvance(bool autoAdvance)
{
    m_autoAdvance = autoAdvance;
}

void MediaPlayerController::setTrackByIndex(const int idx)
{
    const QStringList tracks = (m_mediaMode=="video")?m_videoTracks:m_audioTracks;

    if (((idx+1)>0)&&(idx<tracks.count())) {
        m_trackIndex=idx;
        QString str = tracks.at(m_trackIndex);
//        QJsonDocument doc = QJsonDocument::fromJson(str.toUtf8());
//        QVariantMap m = doc.toVariant().toMap();
//        QString trackStr=QString("file://")+m["Complete name"].toString().trimmed();
//        setTrackTitle(m["Track name"].toString().trimmed());
//        setTrackPerformer(m["Performer"].toString().trimmed());
//        setTrackAlbum(m["Album"].toString().trimmed());
        m_switchingTracks=true;
        setTrack(str);
        m_switchingTracks=false;
        emit trackIndexChanged(m_trackIndex);
//        playTrack(); // automatically start playing when a song is selected, TODO: make this configurable
    }
    else
        qWarning() << Q_FUNC_INFO << "requesting out of range track index" << idx;
}

void MediaPlayerController::setNextTrack()
{
    int idx=m_trackIndex+1;
    if (idx==m_audioTracks.count()&&m_loopAtEnd) idx=0;
    setTrackByIndex(idx);
}

void MediaPlayerController::setPreviousTrack()
{
    setTrackByIndex(m_trackIndex-1);
}
