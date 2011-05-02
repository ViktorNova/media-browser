#-------------------------------------------------
#
# Project created by QtCreator 2011-04-14T12:14:23
#
#-------------------------------------------------

QT = core gui

TARGET = imagescaler
TEMPLATE = lib
VERSION = 1.0.0

DEFINES += IMAGESCALER_LIBRARY

SOURCES += imagescaler.cpp

HEADERS += imagescaler.h\
        ImageScaler_global.h

CONFIG += shared

symbian {
    # Symbian specific definitions
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE84D651E
    TARGET.CAPABILITY = ALL -TCB -AllFiles -DRM
    TARGET.EPOCALLOWDLLDATA = 1

    # Add also support for floating point units, as S^3 devices have it
    # (Not sure, if it really helps in this image scaling case or not...)
    MMP_RULES += "OPTION gcce -march=armv6"
    MMP_RULES += "OPTION gcce -mfpu=vfp"
    MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
    MMP_RULES += "OPTION gcce -marm"

    addFiles.sources = imagescaler.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix {
    maemo5 {
        # Nothing yet.
    } else {
        # Put the library to a some nice place, for the testApp to find it easily.
    DESTDIR += ../Lib
    }
}

win32 {
    DESTDIR += ../Lib
}

# OLD STUFF
#unix:!symbian {
#    maemo5 {
#        target.path = /opt/usr/lib
#    } else {
#        target.path = /usr/local/lib
#    }
#    INSTALLS += target
#}
