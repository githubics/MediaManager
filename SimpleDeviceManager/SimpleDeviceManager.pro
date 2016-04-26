#-------------------------------------------------
#
# Project created by QtCreator 2016-01-29T13:25:06
#
#-------------------------------------------------

QT       += core gui

TARGET = SimpleDeviceManager
TEMPLATE = lib
CONFIG += plugin
CONFIG += c++11

# set this only to simulate a USB drive being inserted
#QMAKE_CXXFLAGS+=-DSIMULATE_USB_DEVICE_PATH=\\\"/media/ics/ROLAND-8G/test_mm1/\\\"
#QMAKE_CXXFLAGS+=-DSIMULATE_USB_DEVICE_PATH=\\\"/Users/rkrause/Downloads/mm_test/\\\"

DESTDIR = ../plugins

SOURCES += \
    simpledevicemanager.cpp

HEADERS += \
../QIcsMediaManager/devicemanagerinterface.h \
    simpledevicemanager.h

DISTFILES += \
    SimpleDeviceManager.json

#unix {
#    target.path = /usr/lib
#    INSTALLS += target
#}
