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
#ifndef MEDIADEVICEINTERFACE
#define MEDIADEVICEINTERFACE

#include <QObject>
#include <QJsonObject>

// forward declare Qt classes
QT_BEGIN_NAMESPACE
class QUrl;
QT_END_NAMESPACE

/** TODO: Document and somehow ensure that the JSon format returned from the
 *  DeviceInterface implementations contains the expected format
 **/
class MediaDeviceInterface : public QObject
{
    Q_OBJECT
public:
    explicit MediaDeviceInterface(QObject * parent = 0) : QObject(parent) {}
    virtual ~MediaDeviceInterface() {}
    /** Possibly Asynchronous call to update the media source list.
     *  When a list is available the signal mediaSourceListUpdate is emitted
     */
    virtual void updateMediaSourceList(const QUrl url) const = 0;

    /** Synchronous call the will return with an updated source list */
    virtual QJsonObject getMediaSourceList(const QUrl url) const = 0;

signals:
    /** Signal emitted when an updated source list is available */
    void mediaSourceListUpdate(const QJsonObject) const;
};

#endif // MEDIADEVICEINTERFACE
