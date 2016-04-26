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
#ifndef SIMPLEDEVICEMANAGER_H
#define SIMPLEDEVICEMANAGER_H

#include "../QIcsMediaManager/devicemanagerinterface.h"
class QFileSystemWatcher;

/** SimpleDeviceManager is a Plugin Interface for a Device Manager
 *  that watches system mounted devices such as USB Pendrives and SD Cards.
 **/
class SimpleDeviceManager : public DeviceManagerInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ics.media-manager.DeviceManagerInterface" FILE "SimpleDeviceManager.json")
    Q_INTERFACES(DeviceManagerInterface)
public:
    SimpleDeviceManager(QObject *parent = 0);

protected slots:
    void deviceChanged();

private:
    QFileSystemWatcher * m_usbWatcher;
    QString m_usbWatchPath;
    QStringList m_devices;
};

#endif // SIMPLEDEVICEMANAGER_H
