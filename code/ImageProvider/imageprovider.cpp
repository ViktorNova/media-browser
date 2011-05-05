#include "imageprovider.h"

#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>
#include <QtGui/QPainter>

// Constants
#define FILE_PREFIX "file:///"
#define REFLECTION_FILE_PREFIX "reflection/" FILE_PREFIX


ImageProvider::ImageProvider():
        //QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)  // Synchronous loading!
        QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
    qDebug() << "Constructing ImageProvider";

    // Create the fading image
    mFadeMask.load(":/gfx/fade_mask.png");
    mFadeMask.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    qDebug() << "Fadeimage format: " << mFadeMask.format();
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
        const QString FilePrefix(FILE_PREFIX);
        const QString ReflectionPrefix(REFLECTION_FILE_PREFIX);

        bool isReflection = false;
        int fileNameStart = id.lastIndexOf("/") + 1;
        QString fileName = id.right(id.length() - fileNameStart);
        QString path("");

        // Check the given URI
        if (id.indexOf(ReflectionPrefix) != -1) {
            const int start = ReflectionPrefix.length();
            qDebug() << "Index of reflection/file:/// is:" << id.indexOf(ReflectionPrefix);
            path = id.mid(start, (fileNameStart-start));
            isReflection = true;
        } else if (id.indexOf(FilePrefix) != -1) {
            qDebug() << "Index of plain file:/// is:" << id.indexOf(FilePrefix);
            const int start = FilePrefix.length();
            path = id.mid(start, (fileNameStart-start));
        } else {
            // The provided uri was of wrong type!
            qDebug() << "The given uri (" << id << ") was incorrect! Panic!";
            Q_ASSERT(false);
        }

        QString imgPath( path + "thumbs/" + fileName);

        // Just for easing debugging...
        qDebug() << "Parsed file name: " << fileName;
        qDebug() << "Parsed path: " << path;
        qDebug() << "Final imgPath: " << imgPath;

        // Create and store the image into the cache.
        QImage img(imgPath);
        if (isReflection) {
            img = maskedImage(img);
        }

        qDebug() << "QImage format is: " << img.format();
        if (!img.isNull()) {
            // TODO: It *might not* be a good idea to store all images into the cache.
            // So, would need some added logic to determine which ones to save, which to discard.
            mCache.insert(id, img);
        }

        return img;
    }
}

QImage ImageProvider::maskedImage(QImage& orig)
{
    qDebug() << "In maskedImage! Format before conversion: " << orig.format();
    // Convert the image to premultiplied alpha format, as it should be
    // faster with transparencies & blending.
    QImage converted = orig.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&converted);
    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawImage(orig.rect(), mFadeMask, mFadeMask.rect());

    return converted;
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
