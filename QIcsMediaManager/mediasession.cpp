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
#include "mediasession.h"
#include "mediaplayerinterface.h"

MediaSession::MediaSession(MediaPlayerInterface *player, QObject *parent)
    : QObject(parent)
    , m_player(player)
{
}

void MediaSession::appendMediaSourcePlaylist(const QString deviceUrl, const QJsonArray playlist)
{
    m_sourcePlaylists[deviceUrl]=playlist;
    rebuildMediaSessionPlaylist();

}

void MediaSession::removeMediaSourcePlaylist(const QString deviceUrl)
{
    // TODO: If the last source playlist has been removed the
    // MediaSession should be removed from the MediaSession list
    // We could schedule a signal event to the manager to delete the
    // session.
    Q_ASSERT(m_sourcePlaylists.contains(deviceUrl));
    m_sourcePlaylists.remove(deviceUrl);
    rebuildMediaSessionPlaylist();
}

void MediaSession::rebuildMediaSessionPlaylist()
{
    // TODO: make this smarter after appending we don't need to rebuild the whole list...
    // however if we sort and filter we should probably sort the whole thing again
// TODO integrate the unsupported file type filtering in a consistent way
//    QSet<QString> supportedFileTypes = m_player->supportedFileSuffixes();
    QStringList  playList;
    foreach (const QJsonArray jsa, m_sourcePlaylists) {
        foreach (QJsonValue jv, jsa) {
            QJsonObject jo=jv.toObject();
            const QString & f=jo["CompleteName"].toString();
//            const QString & s=jo["FileExtension"].toString();
//            if (!supportedFileTypes.contains(s)) {
//                qWarning() << Q_FUNC_INFO << "unknown file suffix" << s << "for" << f;
//                continue;
//            }
            playList << QString("file:%1").arg(f);
        }
    }
    emit mediaSessionPlaylistChanged(playList);
    m_player->setMediaPlaylist(playList);
    qDebug() << Q_FUNC_INFO << playList;
//     TODO: Check state of the player, make sure playing is not interrupted, don't autoplay
//    m_player->stop();
//    m_player->play();
}

const QJsonArray MediaSession::mediaSessionPlaylist() const
{
    QJsonArray result;
    foreach (const QJsonArray jsa, m_sourcePlaylists) {
        foreach (QJsonValue jv, jsa) {
            result.append(jv);
        }
    }
    return result;
}

// TODO: If we can get a playlist from the player we can return it here
//QStringList MediaSession::mediaSessionPlaylist() const
//{
//    return m_player->playList();
//}


