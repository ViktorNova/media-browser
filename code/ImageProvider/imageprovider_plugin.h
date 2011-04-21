#ifndef IMAGEPROVIDER_PLUGIN_H
#define IMAGEPROVIDER_PLUGIN_H

#include <QtDeclarative/QDeclarativeExtensionPlugin>

class ImageProviderPlugin : public QDeclarativeExtensionPlugin
{
    Q_OBJECT

public:
    void registerTypes(const char *uri);
    void initializeEngine(QDeclarativeEngine *engine, const char *uri);

};

#endif // IMAGEPROVIDER_PLUGIN_H

