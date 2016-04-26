#-------------------------------------------------
#
# Project created by QtCreator 2016-02-05T11:00:12
#
#-------------------------------------------------

QT       += core qml quick

TARGET = SimpleUiController
TEMPLATE = lib
CONFIG += plugin c++11

DESTDIR = ../plugins

SOURCES += \
    simpleuicontroller.cpp \
    playlistmodel.cpp \
    coverartprovider.cpp

HEADERS += \
    ../QIcsMediaManager/mediamanagercontrollerinterface.h \
    ../QIcsMediaManager/mediaplayerinterface.h \
    simpleuicontroller.h \
    playlistmodel.h \
    coverartprovider.h

DISTFILES += \
    main.qml \
    MediaButton.qml \
    SimpleUiController.json \
    TimeLineSlider.qml \
    AudioElement.qml \
    PlaylistView.qml \
    VideoElement.qml \
    BlueToothElement.qml

RESOURCES += \
    qml.qrc
