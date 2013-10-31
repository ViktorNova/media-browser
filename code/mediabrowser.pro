#
# Basic Qt configuration
#
QT += declarative
CONFIG += qt qt-components mobility
MOBILITY += gallery

# Version number & version string definition (for using it inside the app)
VERSION = 2.0.0
VERSTR = '\\"$${VERSION}\\"'
DEFINES += VER=\"$${VERSTR}\"

# This removes extra logging from release builds
release {
    DEFINES += QT_NO_DEBUG_OUTPUT
}
#Speed up launching on MeeGo/Harmattan when using applauncherd daemon
#CONFIG += qdeclarative-boostable

# Add more folders to ship with the application, here
qml_sources.source = qml/mediabrowser
qml_sources.target = qml
DEPLOYMENTFOLDERS = qml_sources

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    ImageScaler/imagescaler.cpp \
    ImageProvider/imageprovider.cpp \
    mediabrowsermodel.cpp \
    mediadataobject.cpp
HEADERS += \
    ImageScaler/imagescaler.h \
    ImageProvider/imageprovider.h \
    mediabrowsermodel.h \
    mediadataobject.h
RESOURCES += \
    mediabrowser.qrc

# Extra includes + other files.
INCLUDEPATH += ImageScaler
OTHER_FILES += \
    qtc_packaging/debian_harmattan/rules \
    qtc_packaging/debian_harmattan/README \
    qtc_packaging/debian_harmattan/manifest.aegis \
    qtc_packaging/debian_harmattan/copyright \
    qtc_packaging/debian_harmattan/control \
    qtc_packaging/debian_harmattan/compat \
    qtc_packaging/debian_harmattan/changelog


# Platform specific files and configuration
symbian {
    TARGET.UID3 = 0xE487F346
    # Allow network access on Symbian
    TARGET.CAPABILITY += NetworkServices ReadUserData WriteUserData
    TARGET.EPOCHEAPSIZE = 0x20000 0x4000000

    # Add also support for floating point units, as S^3 devices have it
    MMP_RULES += "OPTION gcce -march=armv6 -mfpu=vfp -mfloat-abi=softfp -marm"

    SOURCES += volumekeys.cpp
    HEADERS += volumekeys.h

    LIBS += -lremconcoreapi -lremconinterfacebase
}
contains(MEEGO_EDITION,harmattan) {
    #QT += opengl
    DEFINES += Q_WS_HARMATTAN
    icon_file.files = mediabrowser.svg
    icon_file.path = /usr/share/icons/hicolor/scalable/apps
    splash.files += qml/mediabrowser/gfx/splash_n9.png
    splash.path = /opt/mediabrowser

    INSTALLS += icon_file splash
}

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
