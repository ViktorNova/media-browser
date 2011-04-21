#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>

#include "qmlapplicationviewer.h"
#include "imagescaler.h"

// Few consts
#ifdef Q_OS_SYMBIAN
    // TODO: Perhaps this should be defined more elegantly?
    static const QString imagePath("E:\\Images\\Backgrounds\\");
#else
    static const QString imagePath("/Images/Backgrounds/");
#endif
static const int thumbSize = 256;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;

    // Set the imageloader to QML context property (cp)
    QDeclarativeContext* context = viewer.rootContext();
    // TODO: Should this actually even be usable from within the QML?
    ImageScaler* thumbCreator = new ImageScaler();
    context->setContextProperty("cpThumbCreator", thumbCreator);
    // Create the thumbnails once.
    // TODO: Perhaps this should be called from withing the QML?
    thumbCreator->scaleImages(imagePath, thumbSize);

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
