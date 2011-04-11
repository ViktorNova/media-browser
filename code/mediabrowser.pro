# Add more folders to ship with the application, here
folder_01.source = qml/mediabrowser
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH = /opt/qtm11/imports

symbian:TARGET.UID3 = 0xE487F346

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices
symbian:TARGET.EPOCHEAPSIZE = 0x20000 0x4000000

#maemo5:QMAKE_LFLAGS += -Wl,-rpath,/opt/qtm11/lib
maemo5:QMAKE_LFLAGS += -Wl
maemo5:QMAKE_LFLAGS_RPATH += /opt/qtm11/lib
maemo5:QMAKE_LFLAGS_RPATH += /opt/qtm11/imports
maemo5:QML_IMPORT_PATH = /opt/qtm11/imports

# Define QMLJSDEBUGGER to allow debugging of QML in debug builds
# (This might significantly increase build time)
# DEFINES += QMLJSDEBUGGER

# If your application uses the Qt Mobility libraries, uncomment
# the following lines and add the respective components to the 
# MOBILITY variable. 
CONFIG += mobility
MOBILITY += gallery

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()
