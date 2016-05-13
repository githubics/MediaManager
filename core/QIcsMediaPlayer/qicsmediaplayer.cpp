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
#ifndef QT_NO_DEBUG_OUTPUT
#define QT_NO_DEBUG_OUTPUT
#endif
#include <QDebug>
#include "qicsmediaplayer.h"


QIcsMediaPlayer::QIcsMediaPlayer(QObject *parent)
    : MediaPlayerInterface(parent),
      m_mediaPlayer(new QMediaPlayer()),
      m_mediaPlaylist(new QMediaPlaylist()),
      m_currentVolume(50)

{
    m_mediaPlayer->setVolume(m_currentVolume);
    m_currentlyPlaying = QMediaPlayer::StoppedState;
    connect(m_mediaPlaylist,&QMediaPlaylist::currentIndexChanged,this,&MediaPlayerInterface::currentTrackIndexChanged);
}

void QIcsMediaPlayer::setMediaPlaylist(const QStringList playList)
{

    foreach(QString song, playList){
        m_mediaPlaylist->addMedia(QUrl(song));
    }

    // TODO:
    // we should then be able to set the playlist directly, question
    // is what happens when the player is playing, how can we prevent it from
    // stopping. the player should be smart enough to only stop if the current
    // song disappears
    m_mediaPlayer->setPlaylist(m_mediaPlaylist);
    //  FIXME
    //        emit mediaPlaylistChanged();

}

void QIcsMediaPlayer::setCurrentTrack(int index) const
{
    if(index < m_mediaPlaylist->mediaCount() && m_mediaPlaylist->currentIndex() != index)
        m_mediaPlaylist->setCurrentIndex(index);
}

const QSet<QString> QIcsMediaPlayer::supportedFileSuffixes() const
{
    QSet<QString> suffixes;
    suffixes << "mp3"<<"wav"<<"ogg";
    return suffixes;
}

void QIcsMediaPlayer::translatePlayStates() const
{
    qDebug() << Q_FUNC_INFO << m_mediaPlayer->state();
    switch (m_mediaPlayer->state()) {
    case QMediaPlayer::StoppedState:
        m_playState=MediaPlayerInterface::Stopped;
        break;
    case QMediaPlayer::PausedState:
        m_playState=MediaPlayerInterface::Paused;
        break;
    case QMediaPlayer::PlayingState:
        m_playState=MediaPlayerInterface::Playing;
        break;
    }
}

void QIcsMediaPlayer::play() const
{
    qDebug() << Q_FUNC_INFO;

    if(m_mediaPlayer->state()!= QMediaPlayer::PlayingState) {
        m_mediaPlayer->play();
        translatePlayStates();
    }
}

void QIcsMediaPlayer::pause() const
{
    qDebug() << Q_FUNC_INFO;
    if(m_mediaPlayer->state() != QMediaPlayer::PausedState) {
        m_mediaPlayer->pause();
        translatePlayStates();
    }
}

void QIcsMediaPlayer::stop() const
{
    qDebug() << Q_FUNC_INFO;
    if(m_mediaPlayer->state() != QMediaPlayer::StoppedState){
        m_mediaPlayer->stop();
        translatePlayStates();
    }
}

void QIcsMediaPlayer::next() const
{
    qDebug() << Q_FUNC_INFO;
    if(m_mediaPlaylist->currentIndex() < m_mediaPlaylist->mediaCount() - 1){
        m_mediaPlaylist->next();
    } else {
        m_mediaPlaylist->setCurrentIndex(0);
    }

}

void QIcsMediaPlayer::previous() const
{
    qDebug() << Q_FUNC_INFO;
    if(m_mediaPlaylist->currentIndex() > 0){
        m_mediaPlaylist->previous();
    } else {
        m_mediaPlaylist->setCurrentIndex(m_mediaPlaylist->mediaCount() - 1);
    }
}


/*void QIcsMediaPlayer::setMediaPlaylist(QMediaPlaylist *newMediaPlaylist)
{
//    m_mediaPlaylist = newMediaPlaylist;
//    m_mediaPlayer->setPlaylist(m_mediaPlaylist);
//    emit mediaPlaylistChanged();
}*/

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
    m_mediaPlayer->setVolume(m_currentVolume);
}

void QIcsMediaPlayer::decrementCurrentVolume()
{
    setCurrentVolume(m_currentVolume - 5);
    m_mediaPlayer->setVolume(m_currentVolume);
}
