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
import QtQuick 2.3
import QtMultimedia 5.0

/*!
  \ingroup media_ui
  */
VideoOutput{
    id: videoOutput
    source: mediaPlayer
    visible: mediaPlayerController.mediaMode === "video"
    anchors.fill: parent //mediaMarquee

    function console_log(){
        var log_output = ""
        for (var i = 0; i < arguments.length; i++){
            log_output += arguments[i] + " "
        }
        console.log(log_output);
    }

    MediaPlayer {
        id: mediaPlayer
        objectName: "mediaPlayer"
        source: mediaPlayerController.track
        autoLoad: true
        autoPlay: false
        onSourceChanged: {
            console_log("source changed to", source)
        }
        onPlaybackStateChanged: mediaPlayerController.setPlaybackState(playbackState)
    }
    Connections {
        target: mediaPlayerController
        onPlay: mediaPlayer.play()
        onPause: mediaPlayer.pause()
        onStop: mediaPlayer.stop()
    }
}
