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
#include "qicsmediaplayer.h"
#include <QGuiApplication>
#include <QQmlApplicationEngine>

QIcsMediaPlayer::QIcsMediaPlayer(QObject *parent)
    : MediaPlayerInterface(parent)
    , m_playerView(new QQuickView())
    , m_audioVideoPlayerController(new MediaPlayerController(this))
{
    Q_INIT_RESOURCE(qml);
    m_playerView->engine()->rootContext()->setContextProperty("mediaPlayerController", m_audioVideoPlayerController);
    m_playerView->setSource(QUrl(QStringLiteral("qrc:/mediaplayer/main.qml")));
    m_playerView->setFlags(Qt::FramelessWindowHint);
}

void QIcsMediaPlayer::setMediaPlaylist(const QStringList playList)
{
    m_audioVideoPlayerController->setVideoTracks(playList);
}

const QSet<QString> QIcsMediaPlayer::supportedFileSuffixes() const
{
    QSet<QString> suffixes;
    suffixes << "mp4"<<"mov"<<"avi"<<"mkv"<<"m4v"<<"webm";
    return suffixes;
}

QQmlEngine * QIcsMediaPlayer::engine() const
{
    return m_engine;
}

QUrl QIcsMediaPlayer::qml() const
{
    return QUrl(QStringLiteral("file:///home/ics/core/QIcsAudioVideoPlayer/main.qml"));
}

QString QIcsMediaPlayer::currentMedia() const
{
    return m_audioVideoPlayerController->track();
}

int QIcsMediaPlayer::currentTrackIndex() const
{
    return m_audioVideoPlayerController->trackIndex();
}


void QIcsMediaPlayer::play() const
{
    if (!m_playerView->isVisible()) m_playerView->show();
    m_audioVideoPlayerController->playTrack();
    translatePlayState();
}

void QIcsMediaPlayer::pause() const
{
    m_audioVideoPlayerController->pauseTrack();
    translatePlayState();
}

void QIcsMediaPlayer::stop() const
{
    m_audioVideoPlayerController->stopTrack();
    if (m_playerView->isVisible()) m_playerView->hide();
    translatePlayState();
}

void QIcsMediaPlayer::next() const
{
    m_audioVideoPlayerController->setNextTrack();
    translatePlayState();

}

void QIcsMediaPlayer::previous() const
{
    m_audioVideoPlayerController->setPreviousTrack();
    translatePlayState();
}

// TODO: Implement adding and removing of playlist components
// without disturbing playback if at all possible
/*void QIcsMediaPlayer::addPlaylistMedia(QUrl mediaItemUrl)
{
//    m_mediaPlaylist->addMedia(mediaItemUrl);
}*/

void QIcsMediaPlayer::setCurrentVolume(quint8 currentVolume)
{
    if(currentVolume == m_currentVolume) return;

    m_currentVolume = currentVolume;
//    emit currentVolumeChanged(m_currentVolume);

}
void QIcsMediaPlayer::incrementCurrentVolume()
{
    setCurrentVolume(m_currentVolume + 5);
//    m_mediaPlayer->setVolume(m_currentVolume);
}

void QIcsMediaPlayer::decrementCurrentVolume()
{
    setCurrentVolume(m_currentVolume - 5);
//    m_mediaPlayer->setVolume(m_currentVolume);
}

void QIcsMediaPlayer::setCurrentTrack(int index) const
{
    if(index < m_mediaPlaylist.count() &&m_audioVideoPlayerController->trackIndex() != index)
        m_audioVideoPlayerController->setTrackByIndex(index);
}

void QIcsMediaPlayer::setVideoRectangle(const QRect videoRect) const
{
    m_playerView->setGeometry(videoRect);
}

void QIcsMediaPlayer::translatePlayState() const
{
    switch (m_audioVideoPlayerController->playbackState()) {
    case MediaPlayerController::StoppedState:
        m_playState=MediaPlayerInterface::Stopped;
        break;
    case MediaPlayerController::PausedState:
        m_playState=MediaPlayerInterface::Paused;
        break;
    case MediaPlayerController::PlayingState:
        m_playState=MediaPlayerInterface::Playing;
        break;
    }
}
