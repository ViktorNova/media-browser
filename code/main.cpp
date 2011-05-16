#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QtCore/QThread>
//#include <QtCore/QtPlugin>

#include "qmlapplicationviewer.h"
#include "imagescaler.h"

// Few constants
static const int thumbSize = 180;
#ifdef Q_OS_SYMBIAN
    // Constants
    #define IMAGE_PATH "E:\\Images\\"
#else
    #define IMAGE_PATH "/Temp/Images/"
#endif

// Trying to get the plugin working..
// NOTE: Doesn't work this way, yet at least.
//Q_IMPORT_PLUGIN(qmlimageproviderplugin)

// Entry point for the QML application
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;

    // Start the ImageScaler to create the thumbs, if necessary. This will be
    // within its own thread in order to avoid blocking the UI.
    QThread scalerThread;
    ImageScaler* thumbCreator = new ImageScaler(QString(IMAGE_PATH), thumbSize);
    thumbCreator->moveToThread(&scalerThread);
    QObject::connect(&scalerThread, SIGNAL(started()), thumbCreator, SLOT(scaleImages()));
    scalerThread.start();

    // TODO: Should this actually even be usable from within the QML?
    // Set the imageloader to QML context property (cp)
    QDeclarativeContext* context = viewer.rootContext();
    context->setContextProperty("cpThumbCreator", thumbCreator);
    // Create the thumbnails once.
    // TODO: Perhaps this should be called from withing the QML?
//    thumbCreator->scaleImages(QString(IMAGE_PATH), thumbSize);

#if defined(Q_WS_MAEMO_5)
    viewer.addImportPath(QString("/opt/qtm11/imports"));
    viewer.addImportPath(QString("/opt/qtm11/imports/QtMobility"));
    viewer.addImportPath(QString("/opt/qtm11/imports/QtMobility/Gallery"));
#endif

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockLandscape);
    viewer.setMainQmlFile(QLatin1String("qml/mediabrowser/main.qml"));
#if defined(QT_SIMULATOR)
    viewer.showFullScreen();
#else
    viewer.showExpanded();
#endif

    return app.exec();
}
