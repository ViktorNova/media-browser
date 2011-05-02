#include "imageprovider.h"

#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>

ImageProvider::ImageProvider():
        //QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)  // Synchronous loading!
        QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
    qDebug() << "Constructing ImageProvider";

    // setFlag(ItemHasNoContents, false);
}

ImageProvider::~ImageProvider()
{
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    // Debug printing.
    if (size) {
        qDebug() << "requestingImage: " << id << ", of size ("
                 << size->width() << "," << size->height() << "), of requestedSize ("
                 << requestedSize.width() << "," << requestedSize.height() << ")";
    }
    else {
        qDebug() << "requestingImage: " << id << ", of requestedSize ("
                 << requestedSize.width() << "," << requestedSize.height() << ")";
    }

    // If an image has already been loaded with the given id, it should be found
    // from the cache. If found, return int directly.
    QHash<QString, QImage>::const_iterator iter = mCache.find(id);
    if (iter != mCache.end()) {
        qDebug() << "Image " << id << " found from the cache!";
        return iter.value();
    }
    else {
        // TESTING TESTING TESTING
        //const QString imgPath("/home/vkes/Temp/Images/Backgrounds/thumbs/" + id);

        int fileNameStart = id.lastIndexOf("/") + 1;
        // TODO: Maybe rather use indexOf("file:///") here instead?
        const int magicStart = 8;   // Comes from the "file:///" in the beginning of the string

        QString fileName = id.right(id.length() - fileNameStart);
        QString path = id.mid(magicStart, (fileNameStart-magicStart));
        QString imgPath( path + "thumbs/" + fileName);

        // Just for easing debugging...
        qDebug() << "Parsed file name: " << fileName;
        qDebug() << "Parsed path: " << path;
        qDebug() << "Final imgPath: " << imgPath;

        // Create and store the image into the cache.
        // TODO: It *might not* be a good idea to store all images into the cache.
        // So, would need some added logic to determine which ones to save, which to discard.
        QImage img(imgPath);
        if (!img.isNull()) {
            mCache.insert(id, img);
        }

        return img;
    }
}

/*
QPixmap ImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    // VKN TODO: Lataa ja palauta täällä bitmäppi!
    qDebug() << "requestPixmap - id: " << id;

    // TESTING TESTING TESTING
    const QString imgPath("/Temp/Images/Backgrounds/thumbs/Forest.jpg");
    QPixmap pixmap(imgPath);
    return pixmap;
}
*/

// End of file
