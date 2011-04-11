#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;
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
