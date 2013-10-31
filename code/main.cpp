/**
 * Copyright (c) 2012 Nokia Corporation.
 */

// Includes
#include <QtGui/QApplication>
#include <QtDeclarative/QDeclarativeContext>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtCore/QThread>

// Internal includes
#include "imagescaler.h"
#include "ImageProvider/imageprovider.h"
#include "mediabrowsermodel.h"
#include "qmlapplicationviewer.h"

#if defined(Q_OS_SYMBIAN)
#include "volumekeys.h"
#endif

// Constants
static const int thumbSize = 240;

#ifdef QT_NO_DEBUG_OUTPUT
    #warning "NO DEBUG INFO WILL BE PRINTED!"
#endif

// Entry point for the QML application.
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    QmlApplicationViewer viewer;
    // Set this attribute in order to avoid drawing the system
    // background unnecessary.
    // TODO: If need be to variate this between S^3 and 5.0 devices, this will
    // need to be variated run-time (e.g. with QDeviceInfo::Version())!
    viewer.setAttribute(Qt::WA_NoSystemBackground);
    viewer.viewport()->setAttribute(Qt::WA_NoSystemBackground);

    // Image provider
    viewer.engine()->addImageProvider("imageprovider", new ImageProvider);

    // Start the ImageScaler to create the thumbs, if necessary. This will be
    // within its own thread in order to avoid blocking the UI.
    QThread scalerThread;
    ImageScaler* thumbCreator = new ImageScaler(thumbSize);
    thumbCreator->moveToThread(&scalerThread);
    QObject::connect(&scalerThread, SIGNAL(started()), thumbCreator, SLOT(scaleImages()));
    QObject::connect(thumbCreator, SIGNAL(scalingDone()), &scalerThread, SLOT(quit()));
    // Run the thumbnail creation once.
    scalerThread.start();

    // Custom gallery model. This model allows both videos and images
    // to be retrieved at the same time.
    QScopedPointer<MediaBrowserModel> galleryModel(new MediaBrowserModel);
    galleryModel->fetchData();
    QDeclarativeContext* context = viewer.rootContext();
    context->setContextProperty("galleryModel", galleryModel.data());

#if defined(Q_OS_SYMBIAN)
    // Context property for listening the HW Volume key events in QML
    QScopedPointer<VolumeKeys> volumeKeys(new VolumeKeys(0));
    context->setContextProperty("volumeKeys", volumeKeys.data());
#endif

    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationLockLandscape);
    viewer.setMainQmlFile(QLatin1String("qml/mediabrowser/main.qml"));
    // Show the QML app & execute the main loop.
    viewer.showExpanded();
    return app.exec();
}
