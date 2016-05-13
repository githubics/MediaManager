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
#ifndef MEDIADEVICEFACTORY
#define MEDIADEVICEFACTORY

#include <QtPlugin>

// forward declare Qt classes
QT_BEGIN_NAMESPACE
class QStringList;
class QUrl;
QT_END_NAMESPACE

class MediaDeviceInterface;

/** MediaDeviceFactory is a Plugin Interface for a Factory
 *  that creates MediaDevice instances.
 *  MediaDevice instances create MediaSourcePlayLists.
 **/
class MediaDeviceFactory
{
public:
    explicit MediaDeviceFactory() {}
    virtual ~MediaDeviceFactory() {}
    virtual MediaDeviceInterface * createMediaDevice(QObject * parent) const = 0 ;
};

QT_BEGIN_NAMESPACE

#define MediaDeviceFactory_iid "com.ics.media-manager.MediaDeviceFactory"

Q_DECLARE_INTERFACE(MediaDeviceFactory, MediaDeviceFactory_iid)

QT_END_NAMESPACE

#endif // MEDIADEVICEFACTORY

