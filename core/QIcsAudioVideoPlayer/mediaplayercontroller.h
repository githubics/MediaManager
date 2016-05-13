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
#ifndef MEDIAPLAYERCONTROLLER_H
#define MEDIAPLAYERCONTROLLER_H

#include <QObject>
#include <QStringList>
/** MediaPlayerController
 *  - interfaces a QML MediaPlayer with C++
 *  - interfaces a QML MediaPlayer with a playlist
 **/
class MediaPlayerController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString track READ track  WRITE setTrack  NOTIFY trackChanged)
    Q_PROPERTY(QString mediaMode READ mediaMode  WRITE setMediaMode  NOTIFY mediaModeChanged)

    Q_PROPERTY(QString trackTitle READ trackTitle WRITE setTrackTitle NOTIFY trackTitleChanged)
    Q_PROPERTY(QString trackPerformer READ trackPerformer WRITE setTrackPerformer NOTIFY trackPerformerChanged)
    Q_PROPERTY(QString trackAlbum READ trackAlbum WRITE setTrackAlbum NOTIFY trackAlbumChanged)

public:
    enum PlaybackState {StoppedState=0, PlayingState, PausedState};
#if QT_VERSION < 0x050500
    Q_ENUMS(PlaybackState)
#else
    Q_ENUM(PlaybackState)
#endif

    explicit MediaPlayerController(QObject *parent = 0);
    QString track() const;

    QStringList audioTracks() const;
    QStringList videoTracks() const;

    PlaybackState playbackState() const;

    bool autoAdvance() const;
    void setAutoAdvance(bool autoAdvance);

    /// This function is supposed to be called only from the QML side, do not call this directly from C++
    Q_INVOKABLE void setPlaybackState(PlaybackState playbackState);
    QString mediaMode() const;

    QString trackTitle() const;
    QString trackPerformer() const;
    QString trackAlbum() const;
    int trackIndex() const;

signals:
    /** These are for the QML side of things */
    void trackChanged(QString track);
    void play();
    void pause();
    void stop();
    /** These are for the C++ backend */
    /** This signal is emitted after the track was changed.
     *  I.e. the QML MediaPlayer will stop playing and this class
     *  sends its stopped signal then this signal will immediately follow.
     **/
    void trackIndexChanged(int);
    /** This signal is emitted when the playbackState changed signal
     *  from the QML MediaPlayer is received and evaluated.
     *  This signal is not emitted while a track change is in progress.
     **/
    void playbackStateChanged(MediaPlayerController::PlaybackState playbackState);
    void mediaModeChanged(QString mediaMode);
    void trackTitleChanged(QString trackTitle);
    void trackPerformerChanged(QString trackPerformer);
    void trackAlbumChanged(QString trackAlbum);

public slots:
    /** These make up the public interface of the class */
    void playTrack();
    void pauseTrack();
    void stopTrack();
    void setTrackByIndex(const int idx);
    void setNextTrack();
    void setPreviousTrack();
    void setAudioTracks(const QStringList &audioTracks);
    void setVideoTracks(const QStringList &videoTracks);
    void setMediaMode(QString mediaMode);

private:
    // these are private implementation functions, do not call them directly
    // call setTrackByIndex instead
    void setTrackTitle(QString trackTitle);
    void setTrackPerformer(QString trackPerformer);
    void setTrackAlbum(QString trackAlbum);
    void setTrack(QString track);
    QString m_track;
    QStringList m_audioTracks;
    QStringList m_videoTracks;
    PlaybackState m_playbackState;
    bool m_autoAdvance;
    bool m_loopAtEnd;
    bool m_switchingTracks;
    int m_trackIndex;
    QString m_mediaMode;
    QString m_trackTitle;
    QString m_trackPerformer;
    QString m_trackAlbum;
};

#endif // MEDIAPLAYERCONTROLLER_H
