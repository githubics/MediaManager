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
import Model 1.0

Item{
    id:timeRect
    height: imageBackground.sourceSize.height
    enabled: controller.currentTrack
    visible: width > 0

    property int textTopMargin: 5
    property int timespan: Number(controller.currentTrack.Duration) / 1000
    property int spanHours: timespan / 3600
    property int spanMins:  (timespan%3600) / 60
    property int spanSecs: (timespan%3600) % 60
    property int elapsedTime: 0
    property int elapsedHours: elapsedTime / 3600
    property int elapsedMins:  (elapsedTime % 3600) / 60
    property int elapsedSecs: (elapsedTime % 3600) % 60

    Timer{
        id:trackTimer
        interval: 1000
        repeat: true
        onTriggered: parent.elapsedTime++
        running: parent.timespan !== undefined && parent.elapsedTime < parent.timespan && controller.playState === Constants.Playing
    }

    Connections
    {
        target: controller
        onCurrentTrackChanged: elapsedTime = 0
        onPlayStateChanged: if(controller.playState === Constants.Stopped) elapsedTime = 0
    }

    Text{
        id: currentTimeText
        property string hourString: String("%1%2").arg(timeRect.elapsedHours < 10 ? "0":"").arg(timeRect.elapsedHours)
        property string minString: String("%1%2").arg(timeRect.elapsedMins < 10 ? "0":"").arg(timeRect.elapsedMins)
        property string secString: String("%1%2").arg(timeRect.elapsedSecs < 10 ? "0":"").arg(timeRect.elapsedSecs)

        text: qsTr("%1:%2:%3").arg(hourString).arg(minString).arg(secString)
        color:"white"
        font.pixelSize: 30
        anchors{
            verticalCenter: parent.verticalCenter
            left: parent.left
        }

    }
    Image {
        id: imageBackground
        source: "qrc:/TimeLine/Assets/timebar/background.png"
        height: sourceSize.height
        anchors {
            verticalCenter: parent.verticalCenter
            left:  currentTimeText.right
            right:   maxTimeText.left
            margins: progressBar.width/2
        }

        smooth: true

        Rectangle {
            id: progressBarBackground
            smooth: true
            radius: 7
            color: "#0383d6" //TODO - average color of cover
            anchors {
                top:    parent.top
                bottom: parent.bottom
                left:   parent.left
                topMargin: 3
                bottomMargin: 8
                leftMargin: 6
                right:progressBar.horizontalCenter
            }
            Rectangle {
                anchors.fill: parent
                radius: parent.radius
                gradient: Gradient {
                    GradientStop { position: 0.7; color: "transparent" }
                    GradientStop { position: 1.2; color: "black" }
                }
            }

        }
            Image {
                id: progressBar
                smooth: true
                source: "qrc:/TimeLine/Assets/timebar/timebar.png"
                anchors{
                    left: imageBackground.left
                    leftMargin: timeRect.elapsedTime * imageBackground.width/timeRect.timespan - width/2
                    verticalCenter: imageBackground.verticalCenter
                }
            }
    }

    Text{
        id: maxTimeText

        property string hourString: String("%1%2").arg(timeRect.spanHours < 10 ? "0":"").arg(timeRect.spanHours)
        property string minString: String("%1%2").arg(timeRect.spanMins < 10 ? "0":"").arg(timeRect.spanMins)
        property string secString: String("%1%2").arg(timeRect.spanSecs < 10 ? "0":"").arg(timeRect.spanSecs)

        text: qsTr("%1:%2:%3").arg(hourString).arg(minString).arg(secString)
        color:"white"
        font.pixelSize: 30
        anchors{
            verticalCenter: parent.verticalCenter
            right: parent.right
        }
    }
}