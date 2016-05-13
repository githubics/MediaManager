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
#include "playlistmodel.h"
#include <QDebug>


PlayListModel::PlayListModel(QObject *parent):QAbstractListModel(parent),
myCurrentIndex(0),
  myCoverArt(CoverArtProvider())
{

    myRoles.insert(Qt::UserRole, "All");
    myRoles.insert(FileRole, "FileName");
    myRoles.insert(TitleRole, "Title");
    myRoles.insert(ArtistRole, "Artist");
    myRoles.insert(AlbumRole, "Album");
    myRoles.insert(DurationRole, "Duration");
    myRoles.insert(ImageRole, "Cover");
}

QVariant PlayListModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || index.row() > rowCount(index.parent()))
        return QVariant();

    switch(role)
    {
        case Qt::UserRole: return myPlayList.at(index.row()); break;
        case FileRole: return myPlayList.at(index.row())["FileName"]; break;
        case TitleRole: return myPlayList.at(index.row())["Title"]; break;
        case ArtistRole: return myPlayList.at(index.row())["Artist"]; break;
        case AlbumRole: return myPlayList.at(index.row())["Album"]; break;
        case DurationRole: return myPlayList.at(index.row())["Duration"]; break;
        case ImageRole: return myPlayList.at(index.row())["Cover"]; break;
    default: return QVariant();
    };
}

Qt::ItemFlags PlayListModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
        return Qt::ItemIsEditable;
    return QAbstractListModel::flags(index)|Qt::ItemIsEditable;
}

QModelIndex PlayListModel::index(int row, int column, const QModelIndex &parent) const
{
    if(column > 0 || row > rowCount(parent))
        return QModelIndex();
    return createIndex(row,column);
}

bool PlayListModel::insertRows(int row, int count, const QModelIndex &parent)
{
    if(row > -1 && row < rowCount(parent) + 1)
    {
        beginInsertRows(parent, row, row + count + 1);
        for(int pos = 0; pos < count; pos++)
            myPlayList.insert(pos + row, QJsonObject());
        endInsertRows();
        return true;
    }
    return false;
}

bool PlayListModel::moveRows(const QModelIndex &sourceParent, int sourceRow, int count, const QModelIndex &destinationParent, int destinationChild)
{
    if(sourceRow + count < rowCount(sourceParent) && destinationChild < rowCount(destinationParent))
    {
        beginMoveRows(sourceParent,sourceRow,sourceRow + count,destinationParent, destinationChild);
        for(int pos = 0; pos < count; pos++)
            myPlayList.move(sourceRow, destinationChild + pos);
        endMoveRows();
        return true;
    }
    return false;
}

bool PlayListModel::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row > -1 && row + count < rowCount(parent))
    {
        beginRemoveRows(parent,row, row + count);
        for(int pos = 0; pos < count; pos++)
            myPlayList.removeAt(row);
        endRemoveRows();
        if(myCurrentIndex >= row )
        {
            int newPos = myCurrentIndex;
            if(newPos < row + count)
                newPos  -= (myCurrentIndex - row);
            else
                newPos -= count;
            if(newPos > myPlayList.count())
                newPos = myPlayList.count() - 1;
            setCurrentIndex(newPos);
        }
        return true;
    }
    return false;
}

QHash<int, QByteArray> PlayListModel::roleNames() const
{
    return myRoles;
}

int PlayListModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return myPlayList.count();
}

bool PlayListModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!index.isValid() || !myRoles.contains(role))
        return false;

    if(role == Qt::UserRole)
        myPlayList[index.row()] = QJsonObject::fromVariantMap(value.toMap());
    else
        myPlayList[index.row()][myRoles[role]] = value.toJsonValue();
    if(index.row() == myCurrentIndex)
        setCurrentIndex(index.row());
    return true;
}

void PlayListModel::revert()
{
    myPlayList.clear();

}

CoverArtProvider* PlayListModel::currentCoverArt()
{
    return &myCoverArt;
}

int PlayListModel::currentIndex() const
{
    return myCurrentIndex;
}

QJsonObject PlayListModel::currentTrack() const
{
    if(myPlayList.count() < 1)
        return QJsonObject();
    return myPlayList[myCurrentIndex];
}

void PlayListModel::setCurrentIndex(int value)
{
    if(value > -1 && value < myPlayList.count() )
    {
        myCurrentIndex = value;
        if(myPlayList[myCurrentIndex]["Cover"] == "Yes")
        {
            myCoverArt.setCoverData(myPlayList[myCurrentIndex]["Cover_Data"].toVariant().toByteArray());
        }
        emit currentTrackChanged();
    }
}
