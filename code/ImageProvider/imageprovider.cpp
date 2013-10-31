/**
 * Copyright (c) 2012 Nokia Corporation.
 */

// Class declaration
#include "imageprovider.h"

#include <QtCore/QDebug>
#include <QtCore/QFile>
#include <QtCore/QTime>
#include <QtDeclarative/qdeclarative.h>
#include <QtGui/QImageReader>
#include <QtGui/QPainter>

// Constants
// NOTE: Removed one last "/" from file:/// -prefixes on MeeGo branch, since the
// file paths will require it and the path format differs from Symbian style.
#define FILE_PREFIX "file://"

#define IMAGE_THUMB "imageThumb/" FILE_PREFIX
#define IMAGE_FULL "imageFull/" FILE_PREFIX
#define IMAGE_REFLECTION "imageReflection/" FILE_PREFIX

#define VIDEO_THUMB "videoThumb/" FILE_PREFIX
#define VIDEO_FULL "videoFull/" FILE_PREFIX
#define VIDEO_REFLECTION "videoReflection/" FILE_PREFIX

#ifdef QT_NO_DEBUG_OUTPUT
    #warning "NO DEBUG INFO WILL BE PRINTED!"
#endif

/*!
  \class ImageProvider
  \brief This class implements the QDeclarativeImageProvider API and can thus
         be used with the QML Image Element's source property in order to provide
         more functionality for getting the images from the disk. The ImageProvider
         provides thumbnails / reflection thumbnails / full images for gallery
         images. For gallery video files a static thumbnail and full screen image
         will be provided.
*/

/*!
  Constructor for the ImageProvider.
*/
ImageProvider::ImageProvider():
        QDeclarativeImageProvider(QDeclarativeImageProvider::Image),
    mImageThumbStr(IMAGE_THUMB),
    mImageFullStr(IMAGE_FULL),
    mImageReflectionStr(IMAGE_REFLECTION),
    mVideoThumbStr(VIDEO_THUMB),
    mVideoFullStr(VIDEO_FULL),
    mVideoReflectionStr(VIDEO_REFLECTION)
{
    qDebug() << "Constructing ImageProvider";

    // Create the fading image.
    mFadeMask.load(":ImageProvider/gfx/fade_alpha_mask.png");
    mFadeMask = mFadeMask.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    qDebug() << "Fadeimage format: " << mFadeMask.format();

    // Create the static video thumbnails.
    mVideoThumb.load(":ImageProvider/gfx/mediabrowser_video_tn.png");
    mVideoThumbReflection.load(":ImageProvider/gfx/mediabrowser_video_tn.png");
    // Combine the alpha channel to create the reflection thumb.
    maskImage(mVideoThumbReflection);
    mVideoFull.load(":ImageProvider/gfx/mediabrowser_video_tn_landscape.png");
    // Draw the white borders around the full video image
    QPainter painter(&mVideoFull);
    painter.setPen(QPen(Qt::white, 8));
    painter.drawRect(mVideoFull.rect());
}

/*!
  Destructor.
*/
ImageProvider::~ImageProvider()
{
}

/*!
  Derived from the QDeclarativeImageProvider. Has to be implemented to provide
  asynchronous loading of the images for QML.

  This method checks the given request id and determines, what kind of image
  should be loaded (i.e. is it thumb/full/reflection for image or video). It
  also calls the image reading method and handles read image caching.
*/
QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    QTime timer;
    timer.start();

    // Debug printing.
    if (size) {
        qDebug() << "--- requestingImage: " << id << ", of size ("
                 << size->width() << "," << size->height() << "), of requestedSize ("
                 << requestedSize.width() << "," << requestedSize.height() << ") ---";
    }
    else {
        qDebug() << "--- requestingImage: " << id << ", of requestedSize ("
                 << requestedSize.width() << "," << requestedSize.height() << ") ---";
    }

    // If an image has already been loaded with the given id, it should be found
    // from the cache. If found, return it directly.
    QTime cacheTimer;
    cacheTimer.start();
    QHash<QString, QImage>::const_iterator iter = mCache.find(id);
    qDebug() << "    Checking the cache took:" << cacheTimer.elapsed() << "ms.";

    if (iter != mCache.end()) {
        qDebug() << "    Image " << id << " found from the cache!";
        qDebug() << "--- The whole requestImage took:" << timer.elapsed() << "ms. ---\n";
        return iter.value();
    }
    else {
        QTime stringTimer;
        stringTimer.start();

        // The thumbnails are saved under a hidden folder.
        const QString privatePath("/home/user/.mediabrowser/thumbs");
        bool isReflection = false;
        bool isFullImage = false;
        int fileNameStart = id.lastIndexOf("/") + 1;
        QString fileName = id.right(id.length() - fileNameStart);
        QString path("");
        QString imgPath("");
        QImage img;

        // The next if-else if-else block will check the the type of the image
        // or video that is requested, and will either set the correct imagePath
        // for loading the image from the disk, or return the static video
        // thumbnail directly (which has been pre-loaded into the memory).
        //
        // First go through the image thumb/reflection/full -cases:
        //
        if (id.indexOf(mImageThumbStr) != -1) {
            qDebug() << "    Index of" << mImageThumbStr << "is:"
                     << id.indexOf(mImageThumbStr);
            const int start = mImageThumbStr.length();
            path = id.mid(start, (fileNameStart-start));
            imgPath = QString(privatePath + path + fileName);

            // TODO: Unfortunately, this will require action from the file system
            // and it will slow down the execution a bit. Other option would
            // be to not show the images not-yet-scaled-to-thumbs yet, but this
            // would require some additional implementation & logic.
            if (!QFile::exists(imgPath)) {
                qDebug() << "    Thumbnail not available, use full image instead";
                imgPath = QString(path + fileName);
            }
        } else if (id.indexOf(mImageReflectionStr) != -1) {
            const int start = mImageReflectionStr.length();
            qDebug() << "    Index of" << mImageReflectionStr << "is:"
                     << id.indexOf(mImageReflectionStr);
            path = id.mid(start, (fileNameStart-start));
            imgPath = QString(privatePath + path + fileName);
            isReflection = true;
        } else if (id.indexOf(mImageFullStr) != -1) {
            const int start = mImageFullStr.length();
            qDebug() << "    Index of" << mImageFullStr << "is:"
                     << id.indexOf(mImageFullStr);
            path = id.mid(start, (fileNameStart-start));
            imgPath = QString(path + fileName);
            isFullImage = true;
        }
        //
        // Then go through the video thumb/reflection/full -cases
        //
        else if (id.indexOf(mVideoThumbStr) != -1) {
            qDebug() << "    Index of" << mVideoThumbStr << "is:" <<
                        id.indexOf(mVideoThumbStr);
            *size = mVideoThumb.size();
            qDebug() << "--- The whole requestImage took:" << timer.elapsed() << "ms."
                     << "(Out of which the id & prefix comparison took:"
                     << stringTimer.elapsed() << "ms.) ---\n";
            return mVideoThumb;
        } else if (id.indexOf(mVideoReflectionStr) != -1) {
            qDebug() << "    Index of" << mVideoReflectionStr << "is:" <<
                        id.indexOf(mVideoReflectionStr);
            *size = mVideoThumbReflection.size();
            qDebug() << "--- The whole requestImage took:" << timer.elapsed() << "ms."
                     << "(Out of which the id & prefix comparison took:"
                     << stringTimer.elapsed() << "ms.) ---\n";
            return mVideoThumbReflection;
        } else if (id.indexOf(mVideoFullStr) != -1) {
            qDebug() << "    Index of" << mVideoFullStr << "is:" <<
                        id.indexOf(mVideoFullStr);
            *size = mVideoFull.size();
            qDebug() << "--- The whole requestImage took:" << timer.elapsed() << "ms."
                     << "(Out of which the id & prefix comparison took:"
                     << stringTimer.elapsed() << "ms.) ---\n";
            return mVideoFull;
        } else {
            // The provided uri was of wrong type!
            qDebug() << "The given uri (" << id << ") was incorrect! Panic!";
            return img;
        }
        qDebug() << "    The id & prefix comparison took:" << stringTimer.elapsed() << "ms.";
        qDebug() << "    The imgPath is:" << imgPath;

        // If we are fetching the reflection image and it wasn't found from the
        // cache, check first if the _plain_ image would exist in the cache.
        if (isReflection) {
            QHash<QString, QImage>::const_iterator iter =
                    mCache.find(mImageThumbStr + path + fileName);

            if (iter != mCache.end()) {
                qDebug() << "    Plain image " << id <<
                            " for reflection FOUND from the cache!";
                img = iter.value();
            } else {
                qDebug() << "    Plain image " << id <<
                            " for reflection NOT found from the cache!" <<
                            " Just read the image from the disk.";
                img = getImage(imgPath, size, requestedSize, isFullImage);
            }
            // Apply the alpha fading mask on the reflection image.
            QTime maskTimer;
            maskTimer.start();
            maskImage(img);
            qDebug() << "    Masking the reflection took:" << maskTimer.elapsed() << "ms.";
        } else {
            // Otherwise just load the image (plain image / full image)
            // from the disk.
            img = getImage(imgPath, size, requestedSize, isFullImage);
        }
        QTime cacheInsertTimer;
        cacheInsertTimer.start();

        if (!img.isNull() && !isFullImage) {
            // TODO: It *might not* be a good idea to store all images into the cache.
            // So, would need some added logic to determine which ones to save, which to discard.
            mCache.insert(id, img);
        }
        qDebug() << "    Inserting to the cache took:" << cacheInsertTimer.elapsed() << "ms.";
        qDebug() << "--- The whole requestImage took:" << timer.elapsed() << "ms. --- \n";

        return img;
    }
}

/*!
  Private method for loading the image from the disk.
  \a path Filepath to the image file which has to be loaded.
  \a size The size of the original image. Will be set when reading the image,
     thus pointer.
  \a requestedSize The size in which the image is wanted. Might cause scaling.
  \a fullImage Set to true, if a full image should be loaded instead of thumbnail.
*/
QImage ImageProvider::getImage(const QString& path, QSize *size,
                               const QSize& requestedSize, bool fullImage)
{
    QTime getImgTimer;
    getImgTimer.start();

    // Create the image reader to handle the scaling of the image
    QImageReader imageReader(path);

    if (!imageReader.canRead()) {
        qDebug() << "ERROR: ImageReader cannot read file from: " << path << "!";
        Q_ASSERT(false);
    }
    qDebug() << "    After creating QImageReader the getImgTimer has elapsed:"
             << getImgTimer.elapsed() << "ms.)";

    // Querying the size of the image from the disk is a bit slow, but it just
    // has to be done in order to calculate the scales etc. correctly.
    QSize imgReaderSize = imageReader.size();
    // Calculate the new dimensions for the full image. Preserve aspect ratio.
    int width = imgReaderSize.width();
    int height = imgReaderSize.height();

    // Set the image's original size, as the QDeclarativeImageProvider::requestImage
    // API requires it to be set.
    *size = imgReaderSize;
    qDebug() << "    Image orig size: (" << width << "x" << height << ")"
             << "(+ so far of getImgTimer elapsed:" << getImgTimer.elapsed() << "ms.)";

    double widthScale = requestedSize.width() / static_cast<double>(width);
    double heightScale = requestedSize.height() / static_cast<double>(height);
    double scalingFactorMax = qMax(heightScale, widthScale);
    double scalingFactorMin = qMin(heightScale, widthScale);
    QImage retImg(requestedSize, QImage::Format_RGB32);

    if (!fullImage) {
        // If reading thumb image, we don't have to do any scaling for it
        // (as the imageScaler has already done it for the thumb), except
        // if the imageScaler hasn't created the image thumb yet.
        const int epsilon = 0.01;
        // If scaling factor isn't ~1, presume that the image has to be scaled.

        if (abs(scalingFactorMax-1) > epsilon) {
            width = scalingFactorMax * width;
            height = scalingFactorMax * height;
            // Set the correct size to which we want the image to be read.
            imageReader.setScaledSize(QSize(width, height));

            qDebug() << "    No thumb generated yet! (Max)Scaling factor was: " << scalingFactorMax
                     << "(+ so far of getImgTimer elapsed:" << getImgTimer.elapsed() << "ms.)";
        }
        // Set the clip rect to the center.
        QPoint topLeft;

        if (width > height) {
            topLeft = QPoint(width/2-requestedSize.width()/2, 0);
        } else {
            topLeft = QPoint(0, height/2-requestedSize.height()/2);
        }
        QRect clipRect(topLeft, requestedSize);
        imageReader.setScaledClipRect(clipRect);
        qDebug() << "    Setting scaled clip rect:" << clipRect;

        // Convert to pre-multiplied alpha format right away. Speeds up the
        // reflection image handling.
        QTime timer;
        timer.start();
        imageReader.read(&retImg);
        qDebug() << "        getImage() thumb image reading took:" << timer.elapsed() << "ms.";
    } else {
        // Full Image uses min scaling factor (so that the image fits to screen
        // on both orientations)
        width = scalingFactorMin * width;
        height = scalingFactorMin * height;
        qDebug() << "    (Min)Scaling factor was: " << scalingFactorMin;

        // Set the correct size to which we want the image to be read. Read it.
        imageReader.setScaledSize(QSize(width, height));
        qDebug() << "    Reading Full Image from: " << path << " with size ("
                 << width << "x" << height << ") !"
                 << "(+ so far of getImgTimer elapsed:" << getImgTimer.elapsed() << "ms.)";
        QTime timer;
        timer.start();
        imageReader.read(&retImg);
        qDebug() << "        getImage() full image reading took:" << timer.elapsed() << "ms.";

        // Draw white borders around the full image.
        qDebug() << "    Drawing white borders around full image";
        QPainter painter(&retImg);
        painter.setPen(QPen(Qt::white, 8));
        painter.drawRect(retImg.rect());
    }

    qDebug() << "    getImage() IN FULL took:" << getImgTimer.elapsed() << "ms.";
    return retImg;
}

/*!
  Private method for converting given image to a gradientally alpha faded
  reflection image.
*/
void ImageProvider::maskImage(QImage& destination)
{
    // Convert the image to a 32bit premultiplied alpha format in order to
    // speed up blending the mask & drawing the image faster on the screen.
    destination = destination.convertToFormat(QImage::Format_ARGB32_Premultiplied);
    QPainter painter(&destination);
    // Draw white rectangle around the image
    painter.setPen(QPen(Qt::white, 4));
    painter.drawRect(destination.rect());

    painter.setCompositionMode(QPainter::CompositionMode_DestinationIn);
    painter.drawImage(destination.rect(), mFadeMask, mFadeMask.rect());
}

// End of file
