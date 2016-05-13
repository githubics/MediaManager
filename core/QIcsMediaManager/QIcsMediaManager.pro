TEMPLATE = app

QT += qml quick multimedia

SOURCES += main.cpp \
    mediamanager.cpp \
    mediasource.cpp \
    mediasession.cpp \
    mediamanagertypes.cpp

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
    devicemanagerinterface.h \
    mediamanagertypes.h

#Plugins.files = $$OUT_PWD/../plugins/lib*.so
#message($$Plugins.files)
#Plugins.path = /opt/QIcsMediaManager/plugins
#INSTALLS += Plugins
