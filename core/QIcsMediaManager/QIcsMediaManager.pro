TEMPLATE = app

QT += qml quick multimedia
CONFIG += c++11

SOURCES += main.cpp \
    mediamanager.cpp \
    mediasource.cpp \
    mediasession.cpp

# RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
#QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)

HEADERS += \
    mediamanager.h \
    mediasource.h \
    mediasession.h \
    mediadeviceinterface.h \
    mediaplayerinterface.h \
    mediadevicefactory.h \
    mediamanagercontrollerinterface.h \
    devicemanagerinterface.h
