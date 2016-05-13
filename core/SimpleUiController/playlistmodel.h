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
#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include <QObject>
#include <QAbstractItemModel>
#include <QJsonObject>
#include "coverartprovider.h"

class PlayListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentTrackChanged)
    Q_PROPERTY(QJsonObject currentTrack READ currentTrack NOTIFY currentTrackChanged)

public:
    enum mediaRoles{FileRole = Qt::UserRole + 1, TitleRole, ArtistRole, AlbumRole, DurationRole, ImageRole};

    explicit PlayListModel(QObject * parent = 0);

    int currentIndex() const;
    QJsonObject currentTrack() const;
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex index(int row, int column, const QModelIndex &parent) const;
    bool insertRows(int row, int count, const QModelIndex &parent);
    bool moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild);
    bool removeRows(int row, int count, const QModelIndex &parent);
    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    void setCurrentIndex(int value);
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    CoverArtProvider *currentCoverArt();

signals:
    void currentTrackChanged();
    void coverArtChanged(CoverArtProvider * coverArt);

public slots:
    void revert();

private:
    QList<QJsonObject> myPlayList;
    QHash<int, QByteArray> myRoles;
    int myCurrentIndex;
    CoverArtProvider myCoverArt;
};

#endif // PLAYLISTMODEL_H
