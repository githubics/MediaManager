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
#ifndef FILESYSTEMDEVICEFACTORY_H
#define FILESYSTEMDEVICEFACTORY_H

#include "../QIcsMediaManager/mediadevicefactory.h"

/** FileSystemDeviceFactory is a Plugin Interface for a Factory
 *  that creates FileSystemDevice instances.
 *  FileSystemDevice instances create MediaSourcePlayLists from a
 *  directory on a mounted file system. The only requirement is that
 *  this file system can be parsed using QDir.
 **/
class FileSystemDeviceFactory : public QObject, public MediaDeviceFactory
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ics.media-manager.MediaDeviceFactory" FILE "FileSystemDevice.json")
    Q_INTERFACES(MediaDeviceFactory)

public:
    FileSystemDeviceFactory(QObject *parent = 0);
    virtual MediaDeviceInterface *createMediaDevice(QObject * parent) const ;
};

#endif // FILESYSTEMDEVICEFACTORY_H
