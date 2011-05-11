#include "imageprovider.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtDeclarative/qdeclarative.h>
#include <QtGui/QPainter>
#include <QtGui/QImageReader>

// Constants
#define FILE_PREFIX "file:///"
#define FULL_IMAGE_FILE_PREFIX "full/" FILE_PREFIX
#define REFLECTION_FILE_PREFIX "reflection/" FILE_PREFIX


ImageProvider::ImageProvider():
        //QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)  // Synchronous loading!
        QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
    qDebug() << "Constructing ImageProvider";

    // Create the fading image
    mFadeMask.load(":/gfx/fade_alpha_mask.png");
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
        const QString FullImagePrefix(FULL_IMAGE_FILE_PREFIX);
        const QString ReflectionPrefix(REFLECTION_FILE_PREFIX);

        bool isReflection = false;
        bool isFullImage = false;
        int fileNameStart = id.lastIndexOf("/") + 1;
        QString fileName = id.right(id.length() - fileNameStart);
        QString path("");
        QString imgPath("");
        QImage img;

        // Check the given URI
        if (id.indexOf(ReflectionPrefix) != -1) {
            const int start = ReflectionPrefix.length();
            qDebug() << "Index of reflection/file:/// is:" << id.indexOf(ReflectionPrefix);
            path = id.mid(start, (fileNameStart-start));
            imgPath = QString( path + "thumbs/" + fileName);
            isReflection = true;
        } else if (id.indexOf(FullImagePrefix) != -1) {
            const int start = FullImagePrefix.length();
            qDebug() << "Index of full/file:/// is:" << id.indexOf(FullImagePrefix);
            path = id.mid(start, (fileNameStart-start));
            imgPath = QString( path + fileName);
            isFullImage = true;
        }
        else if (id.indexOf(FilePrefix) != -1) {
            qDebug() << "Index of plain file:/// is:" << id.indexOf(FilePrefix);
            const int start = FilePrefix.length();
            path = id.mid(start, (fileNameStart-start));
            imgPath = QString( path + "thumbs/" + fileName);
        } else {
            // The provided uri was of wrong type!
            qDebug() << "The given uri (" << id << ") was incorrect! Panic!";
            Q_ASSERT(false);
            return img;
        }

        // Just for easing debugging...
        qDebug() << "Parsed file name: " << fileName;
        qDebug() << "Parsed path: " << path;
        qDebug() << "Final imgPath: " << imgPath;

        // Create and store the image into the cache (if not full image, that is).
        if (!QFile::exists(imgPath)) {
            qDebug() << "The file does not exist!";
            return img;
        }

        img = getImage(imgPath, requestedSize, isFullImage);
        if (isReflection) {
            img = maskedImage(img);
        }

        qDebug() << "QImage format is: " << img.format();
        if (!img.isNull() && !isFullImage) {
            // TODO: It *might not* be a good idea to store all images into the cache.
            // So, would need some added logic to determine which ones to save, which to discard.
            mCache.insert(id, img);
        }

        return img;
    }
}

QImage ImageProvider::maskedImage(QImage& destination)
{
    qDebug() << "In maskedImage! Format before conversion: " << destination.format();
    // Convert the image to premultiplied alpha format, as it should be
    // faster with transparencies & blending.
    QImage converted = destination.convertToFormat(QImage::Format_ARGB32_Premultiplied);

    QPainter painter(&converted);
    // Draw white rectangle around the image
    painter.setPen(QPen(Qt::white, 4));
    painter.drawRect(converted.rect());

    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawImage(destination.rect(), mFadeMask, mFadeMask.rect());

    return converted;
}

QImage ImageProvider::getImage(const QString& path, const QSize& size, bool fullImage)
{
    // Create the image reader to handle the scaling of the image
    QImageReader imageReader(path);
    if (!imageReader.canRead()) {
        qDebug() << "ERROR: ImageReader cannot read file from: " << path << "!";
        Q_ASSERT(false);
    }

    int scaledSize = qMax(size.height(), size.width());

    // Calculate the new dimensions for the thumbnail. Preserve aspect ratio.
    int width = imageReader.size().width();
    int height = imageReader.size().height();

    if (width > height) {
        height = static_cast<double>(scaledSize) / width * height;
        width = scaledSize;
    } else if (width < height) {
        width = static_cast<double>(scaledSize) / height * width;
        height = scaledSize;
    } else {
        width = scaledSize;
        height = scaledSize;
    }

    // Set the correct size to which we want the image to be read. Read it.
    imageReader.setScaledSize(QSize(width, height));
    qDebug() << "Reading Image from: " << path << " with size (" << width << "x" << height << ") !";
    QImage retImg = imageReader.read();

    // If a "full image" was requested, draw white borders around it.
    if (fullImage) {
        qDebug() << "Drawing white borders around full image";
        QPainter painter(&retImg);
        painter.setPen(QPen(Qt::white, 8));
        painter.drawRect(retImg.rect());
    }

    return retImg;
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
