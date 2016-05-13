TEMPLATE = subdirs

SUBDIRS += core \
# ics-plugins \

core.file = core/media-manager.pro
# core.depends += ics-plugins

CONFIG (build_all) {
    message("Compiling MediaManager including static libs for MediaInfoLib and ZenLib")
    SUBDIRS += \
    MediaInfoLib \
    ZenLib

#
    ZenLib.file = ZenLib/Project/Qt/ZenLib.pro
    MediaInfoLib.file = MediaInfoLib/Project/Qt/MediaInfoLib.pro
    MediaInfoLib.depends=ZenLib
    core.depends+=MediaInfoLib
#
    CONFIG (qjsonrpc) {
        SUBDIRS += qjsonrpc
        core.depends+=qjsonrpc
    }

}


