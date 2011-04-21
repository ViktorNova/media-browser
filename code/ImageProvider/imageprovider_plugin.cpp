#include <QtDeclarative/qdeclarative.h>
#include <QtDeclarative/QDeclarativeEngine>
#include <QtCore/QDebug>

#include "imageprovider_plugin.h"
#include "imageprovider.h"

void ImageProviderPlugin::registerTypes(const char *uri)
{
    //QString uriStr(uri);
    //qDebug() << "registerTypes, uri (char*): " << uriStr;
    qDebug() << "registerTypes, uri (char*): " << uri;

    //Q_ASSERT(uri == QLatin1String("com.nokia.ImageProviderExample"));
    //qmlRegisterType<ImageProvider>(uri, 1, 0, "ImageProvider");
}

void ImageProviderPlugin::initializeEngine(QDeclarativeEngine *engine, const char *uri)
{
    //QString uriStr(uri);
    //qDebug() << "initializeEngine, uri: " << uriStr;
    qDebug() << "initializeEngine, uri (char*): " << uri << " engine: " << engine;

    engine->addImageProvider("imageprovider", new ImageProvider);
}

Q_EXPORT_PLUGIN2(qmlimageproviderplugin, ImageProviderPlugin)


// End of file
