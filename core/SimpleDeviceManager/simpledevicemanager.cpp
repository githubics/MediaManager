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

#include "simpledevicemanager.h"

#include <QTimer>
#include <QFileSystemWatcher>
#include <QDir>
#include <QProcessEnvironment>

#if defined Q_OS_MAC
#define USBWATCHPATH QString("/Volumes/")
#elif defined Q_OS_WIN
#define USBWATCHPATH QString("E:/")
#else
#define USBWATCHPATH QString("/media/%1/").arg(pEnv.systemEnvironment().value("USER"))
#endif

SimpleDeviceManager::SimpleDeviceManager(QObject * parent)
    : DeviceManagerInterface(parent)
    , m_usbWatcher(0)
{

    QProcessEnvironment pEnv;
    m_usbWatchPath = USBWATCHPATH;
    m_usbWatcher = new QFileSystemWatcher();
    m_usbWatcher->addPath(m_usbWatchPath);
    connect(m_usbWatcher, &QFileSystemWatcher::directoryChanged, [=](){ QTimer::singleShot(1000, this, &SimpleDeviceManager::deviceChanged); } );
//    QTimer::singleShot(2000,[=](){emit deviceCreated("USBDeviceType",QUrl::fromLocalFile("/Volumes/rkrause/mm2_test"));});
//    QTimer::singleShot(4000,[=](){emit deviceCreated("USBDeviceType",QUrl::fromLocalFile("/media/sv-desktop-106/7DA2-BF26/Bohemian Rhapsody/"));});


#if defined SIMULATE_USB_DEVICE_PATH
    m_usbWatchPath = QString(SIMULATE_USB_DEVICE_PATH);
// This does not work cause the path must not be changed
// we would actually need to move the files somewhere else temporarily
//    QTimer::singleShot(10000, this, [=]() {
//        m_usbWatchPath = QString("/Users/rkrause/Downloads/test_mm1_empty/");
//        emit deviceChanged();
//    });
#endif
    QTimer::singleShot(2000, this, &SimpleDeviceManager::deviceChanged);
}

void SimpleDeviceManager::deviceChanged()
{
    QDir d(m_usbWatchPath);
    qDebug() << Q_FUNC_INFO << m_usbWatchPath;

    if(!d.exists()) return;
    d.setFilter(QDir::Dirs|QDir::NoDotAndDotDot);
    QStringList entryList=d.entryList();


    //check whether a device was removed
    foreach (QString gonedir, m_devices) {
        if (entryList.isEmpty()||!entryList.contains(gonedir)) {
            emit deviceRemoved("USBDeviceType",QUrl::fromLocalFile(m_usbWatchPath+gonedir));
            qDebug() << Q_FUNC_INFO << "device removed:" << gonedir;
            m_devices.removeAll(gonedir);
        }
    }
    // check whether one was inserted
    foreach(QString nudir, entryList) {
        if (nudir.startsWith("Macintosh")) continue;
        if (!m_devices.contains(nudir)) {
            qDebug() << Q_FUNC_INFO << "found device:" << nudir;
            m_devices.append(nudir);
            emit deviceCreated("USBDeviceType",QUrl::fromLocalFile(m_usbWatchPath+nudir));
        }
    }
}
