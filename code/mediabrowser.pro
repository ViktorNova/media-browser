# Add more folders to ship with the application, here
folder_01.source = qml/mediabrowser
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
#QML_IMPORT_PATH = /opt/qtm11/imports
#QTPLUGIN += qmlimageproviderplugin

symbian {
    TARGET.UID3 = 0xE487F346
    # Allow network access on Symbian
    TARGET.CAPABILITY += NetworkServices ReadUserData WriteUserData
    TARGET.EPOCHEAPSIZE = 0x20000 0x4000000
    # ImageScaler library needed for creating the thumbs
    LIBS += -limagescaler
    #LIBS += -lqmlimageproviderplugin
    #TARGET.EPOCALLOWDLLDATA = 1
    # Add also support for floating point units, as S^3 devices have it
    MMP_RULES += "OPTION gcce -march=armv6 -mfpu=vfp -mfloat-abi=softfp -marm"

    # Add also ImageProvider & ImageScaler libraries
    # TODO: Including the ImageProvider plugin does not work yet.
    #imageprovider.sources = ImageProvider/qmlimageproviderplugin.dll ImageProvider/qmldir
    #imageprovider.path = $$QT_IMPORTS_BASE_DIR/ImageProvider
    imagescaler.sources = imagescaler.dll
    imagescaler.path = !:/sys/bin
    DEPLOYMENT += imagescaler #imageprovider
    #DEPLOYMENT_PLUGIN += qmlimageproviderplugin
} else:maemo5 {
    #QMAKE_LFLAGS += -Wl,-rpath,/opt/qtm11/lib
    QT += opengl
    LIBS += -limagescaler
    target.path = /opt/usr/lib
    #QMAKE_LFLAGS += -Wl
    #QMAKE_LFLAGS_RPATH += /opt/qtm11/lib
    #QMAKE_LFLAGS_RPATH += /opt/qtm11/imports
    #QML_IMPORT_PATH = /opt/qtm11/imports
} else:win32 {
    # The desktop Qt for Windows adds +1 to the lib name
    LIBS += -L../code/Lib -limagescaler1
} else:unix {
    LIBS += -L../Lib -limagescaler
}

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += qt mobility
MOBILITY += gallery

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Extra includes
INCLUDEPATH += ImageScaler

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
