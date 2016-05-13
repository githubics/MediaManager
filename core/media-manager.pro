TEMPLATE = subdirs
SUBDIRS += QIcsMediaManager \
QIcsMediaPlayer \
SimpleUiController \
QIcsAudioVideoPlayer \
SimpleDeviceManager \
FileSystemDevice

CONFIG(qjsonrpc) {
    SUBDIRS += \
    QJsonRpcTcpController
}

