#-------------------------------------------------
#
# Project created by QtCreator 2016-01-29T13:25:06
#
#-------------------------------------------------

QT       += core gui

TARGET = FileSystemDevice
TEMPLATE = lib
CONFIG += plugin
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
unix {
# assuming MediaInfoLib and ZenLib checked out parallel to core
    MEDIA_INFO_DIR=../../MediaInfoLib
    ZENLIB_DIR=../../ZenLib
    MEDIA_INFO_LIBS=-L$${MEDIA_INFO_DIR}/Project/Qt -lmediainfo
    ZENLIB_LIBS=-L$${ZENLIB_DIR}/Project/Qt -lzen -lz
}

CONFIG (mediainfo) {
    message("Compiling $$TARGET plugin with MediaInfo option")
    QMAKE_CXXFLAGS += -DUSE_MEDIAINFO -DUNICODE
    INCLUDEPATH += $${ZENLIB_DIR}/Source $${MEDIA_INFO_DIR}/Source
    LIBS += $${MEDIA_INFO_LIBS} $${ZENLIB_LIBS}
}

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
