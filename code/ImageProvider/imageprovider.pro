TEMPLATE = lib
CONFIG += qt plugin
QT += declarative

TARGET = qmlimageproviderplugin
DESTDIR = ImageProvider
#TARGET = $$qtLibraryTarget($$TARGET)

# Input
SOURCES += \
    imageprovider_plugin.cpp \
    imageprovider.cpp

HEADERS += \
    imageprovider_plugin.h \
    imageprovider.h

imgprovider_sources.files = \
    ImageProvider/qmldir
imgprovider_sources.path = ImageProvider

sources.files += $$SOURCES imageprovider.pro untitled.qml
sources.path += .
target.path += ImageProvider

unix:!symbian {
    maemo5 {
        # Nothing yet.
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/local/lib
        # Put the library to a some nice place, for the testApp to find it easily.
        DESTDIR += ../Lib
    }
}

INSTALLS += imgprovider_sources target


symbian {
    # Symbian specific definitions

    # Add also support for floating point units, as S^3 devices have it
    # (Not sure, if it really helps in this case or not...)
    MMP_RULES += "OPTION gcce -march=armv6"
    MMP_RULES += "OPTION gcce -mfpu=vfp"
    MMP_RULES += "OPTION gcce -mfloat-abi=softfp"
    MMP_RULES += "OPTION gcce -marm"

    TARGET.CAPABILITY = ALL -TCB -AllFiles -DRM
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = ImageProvider/qmlimageproviderplugin.dll ImageProvider/qmldir
    addFiles.path = $$QT_IMPORTS_BASE_DIR/ImageProvider
    DEPLOYMENT += addFiles
}

#OTHER_FILES = qmldir

#!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
#    copy_qmldir.target = $$OUT_PWD/qmldir
#    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
#    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
#    QMAKE_EXTRA_TARGETS += copy_qmldir
#    PRE_TARGETDEPS += $$copy_qmldir.target
#}

