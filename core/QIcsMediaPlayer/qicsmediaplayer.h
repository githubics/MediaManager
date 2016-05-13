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
#ifndef QICSMEDIAPLAYER_H
#define QICSMEDIAPLAYER_H

#include <QStringList>

#include "../QIcsMediaManager/mediaplayerinterface.h"

#include <QMediaPlayer>
#include <QMediaPlaylist>
//class QMediaPlaylist;
//class QMediaContent;

/** TODO: This class may have to change names as it should actually
 *  not be a MediaPlayer itself but a Factory that creates MediaPlayer instances.
 *  Otherwise we are restricted e.g. to having exactly one Mp3Player at a time which
 *  might not be sufficient. For proof of architecture this is certainly
 *  acceptable but in a more sophisticated situation one might want multiple mp3 players
 *  running simultaneously and outputting independently to different sinks.
 **/
class QIcsMediaPlayer : public MediaPlayerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ics.media-manager.MediaPlayerInterface" FILE "QIcsMediaPlayer.json")
    Q_INTERFACES(MediaPlayerInterface)

public:
    QIcsMediaPlayer(QObject *parent = 0);
    void setMediaPlaylist(const QStringList playList);
    const QSet<QString> supportedFileSuffixes() const;
    PlayState playState() const {return m_playState;}
    virtual int currentTrackIndex() const {return m_mediaPlaylist->currentIndex();}

protected:

    // TODO: probably want this to be a Q_PROPERTY
    // Why? Is it needed if plugins have no UI?

    mutable QMediaPlayer::State m_currentPlayState;
    QMediaPlayer * m_mediaPlayer;
    mutable QMediaPlaylist * m_mediaPlaylist;
    quint16 m_currentSongIndex;
    QString m_currentlyPlaying;
    mutable PlayState m_playState;
    quint16 m_progress;
    quint8 m_currentVolume;

signals:
    void playStateChanged(PlayState state);
    void currentTrackIndexChanged(int index);
    void currentTrackChanged(QString track);
    void progressChanged(int progress);

public slots:
    void play() const;
    void pause() const;
    void stop() const;
    void next() const;
    void previous() const;
//    void addPlaylistMedia(QUrl mediaItemUrl);
    void setCurrentVolume(quint8 currentVolume);
    void incrementCurrentVolume();
    void decrementCurrentVolume();
    void setCurrentTrack(int index) const;
//    void setVideoRect(QRect videoRect) const;

private:
    void translatePlayStates() const;
};

#endif // QICSMEDIAPLAYER_H
