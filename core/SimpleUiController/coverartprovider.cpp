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
#include "coverartprovider.h"
#include <QDebug>
#include <QDataStream>
#include <QFile>
#include <QFileInfo>

CoverArtProvider::CoverArtProvider(): QQuickImageProvider(QQuickImageProvider::Pixmap, QQmlImageProviderBase::ForceAsynchronousImageLoading)
  , myCoverData(QByteArray())
{
}

void CoverArtProvider::setCoverData(QByteArray data)
{
    if(myCoverData != data)
        myCoverData = QByteArray::fromBase64(data);
}


QPixmap CoverArtProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    Q_UNUSED(&id);
    QPixmap coverArt= QPixmap(requestedSize.width() > 0 ? requestedSize.width() : size->width(),
                                      requestedSize.height() > 0 ? requestedSize.height() : size->height());
    coverArt.loadFromData(myCoverData);
    return coverArt;
}