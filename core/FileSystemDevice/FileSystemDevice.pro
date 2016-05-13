#-------------------------------------------------
#
# Project created by QtCreator 2016-01-29T13:25:06
#
#-------------------------------------------------

QT       += core gui

TARGET = FileSystemDevice
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11
# set this if mediainfo is present
CONFIG += mediainfo

DESTDIR = ../plugins

SOURCES += \
    filesystemdevicefactory.cpp \
    filesystemdevice.cpp

HEADERS += \
../QIcsMediaManager/mediadeviceinterface.h \
    filesystemdevicefactory.h \
    filesystemdevice.h

DISTFILES += FileSystemDevice.json

windows: !unix {
    MEDIA_INFO_DIR=/Development/MediaInfoLib/Source
    ZENLIB_DIR=/Development/ZenLib/Source
}
unix:!macx {
    MEDIA_INFO_DIR=$$(HOME)/MediaInfo/MediaInfoLib/Source
    ZENLIB_DIR=$$(HOME)/MediaInfo/ZenLib/Source
}
macx {
    MEDIA_INFO_DIR=/Users/rkrause/src/cpp/MediaInfo/MediaInfoLib/Source
    ZENLIB_DIR=/Users/rkrause/src/cpp/MediaInfo/ZenLib/Source
}

CONFIG (mediainfo) {
    message("Compiling $$TARGET plugin with MediaInfo option")
    QMAKE_CXXFLAGS += -DUSE_MEDIAINFO -DUNICODE

#    message("MEDIA_INFO_DIR=$$MEDIA_INFO_DIR")
    INCLUDEPATH += $${ZENLIB_DIR}
    INCLUDEPATH += $${MEDIA_INFO_DIR}

    LIBS += -L$${MEDIA_INFO_DIR}/Library -lMediaInfo
    LIBS += -L$${ZENLIB_DIR}/Library -lZenLib -lz
}

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
