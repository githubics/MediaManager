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
#ifndef DEVICEMANAGERINTERFACE
#define DEVICEMANAGERINTERFACE

#include <QObject>
#include <QUrl>
class QString;

/** DeviceManagerInterface is a Plugin Interface for DeviceManagers
 *  that detect MediaDevices which contain Media that can be indexed
 *  by a suitable MediaDevice.
 **/
class DeviceManagerInterface : public QObject
{
    Q_OBJECT
public:

    explicit DeviceManagerInterface(QObject * parent=0) : QObject(parent) {}
    virtual ~DeviceManagerInterface() {}

signals:
    void deviceCreated(const QString mediaDeviceType, const QUrl mediaDevicePath) const;
    void deviceRemoved(const QString mediaDeviceType, const QUrl mediaDevicePath) const;
};

#define DeviceManagerInterface_iid "com.ics.media-manager.DeviceManagerInterface"
Q_DECLARE_INTERFACE(DeviceManagerInterface, DeviceManagerInterface_iid)


#endif // DEVICEMANAGERINTERFACE

