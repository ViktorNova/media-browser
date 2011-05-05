#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeImageProvider>

class QImage;

class ImageProvider : public QDeclarativeImageProvider
{
    Q_DISABLE_COPY(ImageProvider)

public:
    ImageProvider();
    ~ImageProvider();

    // Asynchronous loading of images
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

    // Synchronous loading of images
    //QPixmap requestPixmap(const QString& id, QSize* size, const QSize& requestedSize);

private: // Methods
    QImage maskedImage(QImage& orig);

private:
    // For caching images internally
    QHash<QString, QImage> mCache;

    // For reflection
    QImage mFadeMask;
};

//QML_DECLARE_TYPE(ImageProvider)

#endif // IMAGEPROVIDER_H

