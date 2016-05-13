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

Rectangle{
    id: root
    visible: true
    width: 800
    height: 480

    Image {
        id: backgroundImage
        anchors.fill: parent
        source: "Assets/background.png"
    }


    Column{
        id: sessionColumn
        width: 100
        anchors{
            top: root.top
            bottom: root.bottom
            left: root.left
            margins: 5
        }
        spacing: 10

        MediaButton{
            id: audioButton
            buttonLabel: qsTr("Audio")
            height: 100
            anchors{
                left: sessionColumn.left
                right: sessionColumn.right
            }
            enabled: controller.activeMediaSession !== "AudioFile"
            onClicked: controller.activeMediaSession = "AudioFile"
            activated: !enabled

        }

        MediaButton{
            id: videoButton
            buttonLabel: qsTr("Video")
            height: 100
            anchors{
                left: sessionColumn.left
                right: sessionColumn.right
            }
            enabled: controller.activeMediaSession !== "VideoFile"
            onClicked: controller.activeMediaSession = "VideoFile"
            activated: !enabled
        }

        MediaButton{
            id: bluetoothButton
            buttonLabel: qsTr("Bluetooth")
            height: 100
            anchors{
                left: sessionColumn.left
                right: sessionColumn.right
            }
            enabled: controller.activeMediaSession !== "EndType"
            onClicked: controller.activeMediaSession = "EndType"
            activated: !enabled
        }
    }


    Rectangle{
        id: currentSessionRect
        clip: true
        color: "transparent"
        anchors{
            top: root.top
            bottom: root.bottom
            left: sessionColumn.right
            right: sessionColumn.right
            margins: 5
            rightMargin: 0
        }

        Item{
            id: playerVisualRect
            anchors{
                top: parent.top
                right:parent.right
                left: parent.left
                bottom: trackSlider.top
                margins: 5
            }
            AudioElement {
                id:audioSpace
                anchors.fill: parent
                visible: false
                opacity: 0
            }

            VideoElement {
                id: videoSpace
                anchors.fill: parent
                visible: false
                opacity: 0
                Connections{
                    target: controller
                    onActiveMediaSessionChanged:{
                        if(!root.transitions.running && controller.activeMediaSession === "VideoFile"){
                            var view = root.mapFromItem(videoSpace.vidRect)
                            // TODO: Would like to revisit this and see if it it the UI that should determine the size of the
                            // player or whether the controller should have some more to say about this.
                            // In a sense I'd like to keep the UI as shallow as possible and not making decisions like this
                            // but then we would still need to figure out who would know the correct size for the video rectangle
                            controller.setVideoRectangle( view.x, view.y, videoSpace.vidRect.height, videoSpace.vidRect.width)
                        }
                    }
                }
            }
            BlueToothElement {
                id:blueSpace
                anchors.fill: parent
                visible: false
                opacity: 0
            }

            states:[
                State{
                    name: "audio"
                    when: controller.activeMediaSession === "AudioFile"
                    PropertyChanges{
                        target: audioSpace
                        visible: true
                        opacity: 1.0
                    }
                },
                    State{
                        name: "video"
                        when: controller.activeMediaSession === "VideoFile"
                        PropertyChanges{
                            target: videoSpace
                            visible: true
                            opacity: 1.0
                        }
                },
                State{
                    name: "blue"
                    when: controller.activeMediaSession === "EndType"
                    PropertyChanges{
                        target: blueSpace
                        visible: true
                        opacity: 1.0
                    }
                }
            ]

            transitions:[
                Transition{
                    from: "*"
                    to: "*"
                    reversible: true
                    PropertyAnimation{
                        properties: "opacity"
                        duration: 500
                    }
                }
            ]


        }

        TimeLineSlider{
            id: trackSlider
            visible: controller.activeMediaSession !== "EndType"
            anchors{
                bottom: playbackRow.top
                left: parent.left
                right: parent.right
                margins: 5
            }
        }

        Row{
            id: playbackRow
            enabled:controller.activeMediaSession !== "EndType"
            anchors{
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
            }
            height: 75
            spacing: 15

            MediaButton{
                id: backButton
                icon: "qrc:/Buttons/previous.png"
                height: 75
                width: 125
                onClicked:{
                    controller.previous()
                }
            }

            MediaButton{
                id: playButton
                icon: "qrc:/Buttons/play.png"
                height: 75
                width: 125
                visible: controller.playState !== Constants.Playing
                onClicked:{
                    controller.play()
                }
            }

            MediaButton{
                id: pauseButton
                icon: "qrc:/Buttons/pause.png"
                height: 75
                width: 125
                visible: !playButton.visible
                onClicked:{
                    controller.pause()
                }
            }

            MediaButton{
                id: stopButton
                icon: "qrc:/Buttons/stop.png"
                height: 75
                width: 125
                onClicked:{
                    controller.stop()
                }
            }

            MediaButton{
                id: nextButton
                icon: "qrc:/Buttons/next.png"
                height: 75
                width: 125
                onClicked:{
                    controller.next()
                }
            }
        }
    }


    states:[
        State{
            name: "open"
            when: controller.activeMediaSession !== "NoType"
            AnchorChanges{
                target: currentSessionRect
                anchors.right: root.right
            }
            PropertyChanges{
                target: currentSessionRect
                anchors.rightMargin: 5
            }
        }
    ]

    transitions:[
        Transition{
            from: ""
            to: "open"
            reversible: true
            AnchorAnimation{
                duration: 500
            }
        }
    ]
}

