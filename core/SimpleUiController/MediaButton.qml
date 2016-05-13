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
import QtGraphicalEffects 1.0

    Image {
        id: backingImage
        property alias buttonLabel : label.text
        property alias icon: iconImage.source
        property bool activated: false
        signal clicked()

        source: mouseArea.pressed ? "qrc:/Buttons/pressed.png" : "qrc:/Buttons/button.png"

        Image {
            id: iconImage
            fillMode: Image.PreserveAspectFit
            anchors.fill: parent
            anchors.margins: 40 * parent.height/parent.sourceSize.height
        }
        Text {
            id: label
            anchors.centerIn: parent
            color:"white"
            font.pixelSize: 15

        }
        Glow{
            id:onGlow
            anchors.fill: label
            transparentBorder: true
            source: label
            color: "green"
            radius: 0
            samples: 32
            spread: 0.0
            visible: true
        }


        MouseArea{
            id:mouseArea
            anchors.fill: parent
            onClicked: backingImage.clicked()
        }

        states: [
            State {
                name: "on"
                when:activated
                PropertyChanges {
                    target: onGlow
                    radius: 32
                    spread: .75
                }
                PropertyChanges {
                    target: label
                    color: "lightGreen"
                }

            }
        ]
        transitions: Transition {
            reversible: true
            SequentialAnimation {
                ColorAnimation {
                    duration: 500
                }
                NumberAnimation {
                    properties: "radius, spread"
                    duration: 1000
                }
            }
        }

    }

