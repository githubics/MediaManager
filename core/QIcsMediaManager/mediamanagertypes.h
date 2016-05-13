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
#ifndef MEDIAMANAGERTYPES_H
#define MEDIAMANAGERTYPES_H
#include <QHash>
#include <QStringList>
#include <QObject>

class mmTypes
{
    Q_GADGET
public:
    /** Known MediaDevice for which we can load plugins
     *  IMPORTANT: If you change these you must change the corresponding line in mediamanagertypes.cpp
     **/
    enum MediaDevice { NoDevice=0, USBDevice, IPodDevice, EndDevice };

    /** Known MediaTypes for which player plugins have been found
     *  IMPORTANT: If you change these you must change the corresponding line in mediamanagertypes.cpp
     **/
    enum MediaType {NoType=0, AudioFile, VideoFile, EndType };

    /** MediaPlayer PlayState know to the MediaManager and its plugins
     *  IMPORTANT: If you change these you must change the corresponding line in mediamanagertypes.cpp
     */
    enum PlayState {Playing = 0, Paused, Stopped};

#if QT_VERSION < 0x050500
    Q_ENUMS(MediaDevice)
    Q_ENUMS(MediaType)
    Q_ENUMS(PlayState)
#else
    Q_ENUM(MediaDevice)
    Q_ENUM(MediaType)
    Q_ENUM(PlayState)
#endif

private:
    // FIXME: Use Qt Q_ENUM macro and MetaObject for conversion between names and enums below
    struct Enumeration {
        Enumeration(const QStringList args)
        {
            int i=0;
            foreach (const QString key, args) {_[key]=i++;}
        }
        const Enumeration & operator= (const QStringList args)
        {
            int i=0;
            foreach (const QString key, args) {_[key]=i++;}
            return *this;
        }

        QHash<QString,int> _;
        const QString operator() (const int n) const
        {
            return _.key(n,_.key(0));
        }
        int operator() (const QString s) const
        {
            return _.value(s,0);
        }
    };
public:
    static Enumeration Device;
    static Enumeration Media;
};

#endif // MEDIAMANAGERTYPES_H
