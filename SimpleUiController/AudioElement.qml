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
import QtQuick 2.0

Item {
    id: audioRoot
    Rectangle {
        id: albumCover
        anchors {
            top: parent.top
            bottom: parent.bottom
            left: parent.left
            right: playListRect.left
            rightMargin: 5
        }
        Image{
            id: coverImage
            anchors.fill: parent
            asynchronous: true
            visible: source != ""
            source: ( controller.currentTrack.Cover === "Yes") ? "image://coverart/" + controller.currentIndex : ""
        }
    }
    Rectangle {
        id: playListRect
        anchors {
            top: parent.top
            bottom: parent.bottom
            right: parent.right
        }
        width: parent.width/3
        border.width: 1
        color: "black"
        clip: true
        PlaylistView {
            id: playListView
            model: controller.audioPlaylist
        }
    }
}

