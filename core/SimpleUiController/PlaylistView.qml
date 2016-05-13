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

ListView{
    id: playlistView
    snapMode: ListView.SnapToItem
    anchors{
        fill: parent
        margins: 10
    }
    spacing: 10
    delegate: Item{
        height: 37
        anchors{
            left: parent.left
            right: parent.right
            margins: 5
        }
        clip: true
        Text{
            anchors{
                left: parent.left
                right: parent.right
                bottom: parent.verticalCenter
                bottomMargin: 1
            }

            text: (modelData.Title !== "")? modelData.Title : modelData.FileName
            verticalAlignment: Text.AlignVCenter
            color: "white"
            height: 20
            fontSizeMode: Text.VerticalFit
            font.bold: index == controller.currentIndex
        }

        Text{
            anchors{
                left: parent.left
                right: parent.horizontalCenter
                top: parent.verticalCenter
                topMargin: 1
            }

            text: String("%1 - %2").arg(modelData.Artist).arg(modelData.Album)
            verticalAlignment: Text.AlignVCenter
            color: "gray"
            height: 15
            fontSizeMode: Text.VerticalFit
            font.bold: index == controller.currentIndex
        }
        MouseArea{
            anchors.fill: parent
            onClicked: controller.currentIndex = index
        }
    }
    currentIndex: controller.currentIndex
    highlight: Component{
        Rectangle{
            width: playlistView.width
            height: playlistView.currentItem.height + playlistView.spacing
            y: playlistView.currentItem.y - (playlistView.spacing/2)
            radius: (playlistView.spacing/2)
            color: "lightsteelblue"
            Behavior on y{
                SpringAnimation{
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }
    highlightFollowsCurrentItem: false
}