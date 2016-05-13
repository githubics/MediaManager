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
#ifndef MEDIASESSION_H
#define MEDIASESSION_H

#include <QMap>
#include <QObject>
#include <QJsonArray>
class MediaPlayerInterface;

/** MediaSession manages a MediaSession playlist which is composed of contributions
 *  from various MediaSouce Playlists.
 *  Each MediaSession only manages playlists of media of the same MediaType.
 *
 *  The function rebuildMediaSessionPlaylist assembled the playlist.
 *  It is in this function where sorting and filtering should be implemented.
 *  MediaSession does not store the playlist resulting from assembling instead
 *  it sets it in the Media Player and that's all.
 *
 *  MediaSession can however return an assembled list of media tracks as a JsonArray.
 *  This list will contain all media information.
 **/
class MediaSession : public QObject
{
    Q_OBJECT

public:
    explicit MediaSession(MediaPlayerInterface * player, QObject *parent = 0);

    /** Append a playlist from a MediaSource to this MediaSession
     *  This function keeps track of the playlists coming from the deviceUrl.
     */
    void appendMediaSourcePlaylist(const QString deviceUrl, const QJsonArray playlist);
    /** Remove the playlist from deviceUrl and rebuilt the playlist for the MediaPlayer */
    void removeMediaSourcePlaylist(const QString deviceUrl);

// TODO: If we can get a playlist from the player we can return it here
//    /** Returns the current MediaSession playlist as used by the corresponding MediaPlayer */
//    QStringList mediaSessionPlaylist() const;
    /** Access to the MediaPlayer */
    const MediaPlayerInterface * player() const { return m_player;}

    /** This function returns the concatenated playlist with all the meta-information as a
     *  QJsonArray. This list is not cached to calling this function will sort, filter etc
     *  and return the full result of this effort.
     *  TODO: we should evaluate the performance of this function with about 10k songs
     *  then measure memory consumption vs speed and decide whether to cache this
     **/
    const QJsonArray mediaSessionPlaylist() const ;

signals:
    void mediaSessionPlaylistChanged(QStringList mediaSessionPlaylist);

private slots:

private:
    /** This function rebuild the MediaSession playlist that is then sent to the player
     *  immediately, the list is not stored, if you need it then get it from the player.
     *  There is however a signal emitted that carries the playlist. Might be kind of
     *  heavy and slow if the signal is connected to a slot.
     *  Filtering by file ending is always applied.
     *  TODO: Make this configurable
     *  TODO: Implement sorting and filtering, these should either be plugins or
     *  otherwise be made configurable end extensible. This is where playlist intelligence
     *  should / could be implemented.
     **/
    void rebuildMediaSessionPlaylist();

    /** MediaPlayer
     **/
    MediaPlayerInterface * m_player;

    /** Cache all contributing source playlists.
     *  Used to rebuild the session playlist fast after adding and removing.
     **/
    QMap<QString,QJsonArray> m_sourcePlaylists;
};

#endif // MEDIASESSION_H
