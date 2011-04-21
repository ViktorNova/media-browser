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

INSTALLS += sources imgprovider_sources target

symbian {
    #Symbian specific definitions
#    MMP_RULES += EXPORTUNFROZEN
#    TARGET.UID3 = 0xE84D651F
#    TARGET.CAPABILITY = #ALL -TCB -AllFiles -DRM
    TARGET.EPOCALLOWDLLDATA = 1
#    addFiles.sources = qmlimageproviderplugin.dll qmldir
#    addFiles.path = ImageProvider
    addFiles.sources = ImageProvider/qmlimageproviderplugin.dll ImageProvider/qmldir
    addFiles.path = ImageProvider
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

