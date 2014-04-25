/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

// Class declaration
#include "imagescaler.h"

// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QFileInfoList>
#include <QtGui/QImage>
#include <QtGui/QImageReader>

#include <QGalleryResultSet>

#ifdef QT_NO_DEBUG_OUTPUT
    #warning "NO DEBUG INFO WILL BE PRINTED!"
#endif

// Constants
#define THUMBS_DIR "thumbs"
#define THUMBS_DIR_WITH_PREFIX "/" THUMBS_DIR

// Default length for height or width of the thumbnail.
static const int DefaultThumbSize = 240;

/*!
  \class ImageScaler
  \brief This class walks through all the image files (gotten from the
         QDocumentGallery) and scales them down to a square thumbnail.
         This is done once during startup. Also, a separate thread is
         reserved for the ImageScaler, as it would otherwise lock the UI
         during startup for some time.
*/

/*!
  Constructs the ImageScaler. The path and thumbSize HAVE TO BE given.
  @param thumbSize The maximum length that the longer dimension (width/height) will receive.
*/
ImageScaler::ImageScaler(const int thumbSize, QObject* parent):
    QObject(parent),
    mThumbSize(thumbSize),
    m_documentGallery(0),
    m_galleryQuery(0)
{
    // Do some error checking for given thumbsize & conversion path.
    if (mThumbSize <= 0) {
        qDebug() << "ThumbSize INVALID, using default: " << DefaultThumbSize;
        mThumbSize = DefaultThumbSize;
    }
}

/*!
  Destructor.
*/
ImageScaler::~ImageScaler()
{
    delete m_documentGallery;
    delete m_galleryQuery;
    m_documentGallery = 0;
    m_galleryQuery = 0;
}

/*!
  Slot that starts initialization of the Document Gallery queries.
*/
void ImageScaler::init()
{
    m_documentGallery = new QDocumentGallery(this);
    m_galleryQuery = new QGalleryQueryRequest(m_documentGallery, this);
    connect(m_galleryQuery, SIGNAL(finished()),
            this, SLOT(requestFinished()));
    connect(m_galleryQuery, SIGNAL(error(int,QString)),
            this, SLOT(requestError()));

    qDebug() << "ImageScaler object constructed & initialized!";
}

/*!
  Method to convert the images to thumbs in the specified folder.
  Scales the images within the given path to thumbnails into "/thumbs" subfolder.
*/
bool ImageScaler::scaleImages()
{
    if (m_documentGallery == 0) {
        init();
    }

    fetchImageResults();
    return true;
}

/*!
  Worker method, checks if the thumb does not exist and starts saving to file.
*/
bool ImageScaler::convertToThumb(const QFileInfo& info)
{
    bool retVal = false;

    // The thumbnails are saved under a hidden folder in order
    // to avoid showing the thumbnails in the Media Gallery.
    const QString privatePath("/home/user/.mediabrowser/thumbs");
    QDir saveDir(privatePath + info.path());

    if (!saveDir.exists()) {
        if (!saveDir.mkpath(saveDir.path())) {
            qDebug() << "Thumbs folder creation failed. Quitting!";
            return false;
        }
    }
    // Create the thumb file path to the private save folder.
    QString saveName = saveDir.path() + "/" + info.fileName();

    // Check if the file already exists
    if (QFile::exists(saveName)) {
        qDebug() << "File: " << saveName << " already exist!";
    } else {
        // Does not exist yet, read, scale & save the image!
        retVal = saveImage(info, saveName);
    }

    return retVal;
}

/*!
  Worker method, that does the actual image conversion & saving to file.
*/
bool ImageScaler::saveImage(const QFileInfo& info, const QString& saveName)
{
    bool retVal = false;
    // Create the image reader to handle the downscaling of the image
    QImageReader imageReader(info.filePath());

    if (imageReader.canRead()) {
        // Calculate the new dimensions for the thumbnail. Preserve aspect ratio.
        QSize imgReaderSize = imageReader.size();
        int width = imgReaderSize.width();
        int height = imgReaderSize.height();

        if (width > height) {
            width = width * mThumbSize / static_cast<double>(height);
            height = mThumbSize;
        } else if (width < height) {
            height = height * mThumbSize / static_cast<double>(width);
            width = mThumbSize;
        } else {
            width = mThumbSize;
            height = mThumbSize;
        }
        QPoint topLeft;

        if (width > height) {
            topLeft = QPoint(width/2-mThumbSize/2, 0);
        } else {
            topLeft = QPoint(0, height/2-mThumbSize/2);
        }
        // Set the correct size to which we want the image to be read. Read it.
        imageReader.setScaledSize(QSize(width, height));
        QRect scaledClipRect(topLeft, QSize(mThumbSize, mThumbSize));
        imageReader.setScaledClipRect(scaledClipRect);
        QImage thumbnail = imageReader.read();
        // Convert & save the image to disk right away!
        retVal = thumbnail.save(saveName);

        qDebug() << "Image: " << info.fileName() << " saved to " << saveName
                 << ", with scaled size:" << imageReader.scaledSize().width() << "x"
                 << imageReader.scaledSize().height()
                 << "and with scaledClipRect:" << scaledClipRect
                 << ", img.save()'s retVal was: " << retVal << "!";
    } else {
        qDebug() << "ERROR: ImageReader cannot read file from: " << info.filePath() << "!";
    }

    return retVal;
}

/*!
  Method for starting the Document Gallery query.
*/
void ImageScaler::fetchImageResults()
{
    if (!m_galleryQuery->isSupported()) {
        qDebug() << "Request not supported";
        return;
    }
    m_galleryQuery->setRootType(QDocumentGallery::Image);
    m_galleryQuery->execute();
}

/*!
  Slot, that's being called when the QGalleryQueryRequest finishes.
*/
void ImageScaler::requestFinished()
{
    qDebug() << "ImageScaler::requestFinished";

    QGalleryResultSet *resultSet = m_galleryQuery->resultSet();
    qDebug() << "Nbr. of results: " << resultSet->itemCount();

    int dbgCount = 0;
    resultSet->fetchFirst();

    for (int i=0; i < resultSet->itemCount(); i++) {
        QFileInfo fileInfo(resultSet->itemUrl().toLocalFile());

        if (fileInfo.isFile() &&
                fileInfo.filePath().indexOf(THUMBS_DIR_WITH_PREFIX) < 0) {
            convertToThumb(fileInfo);
            dbgCount++;
        } else {
            qDebug() << "Skipped " << resultSet->itemUrl().toLocalFile();
        }
        resultSet->fetchNext();
    }
    qDebug() << "Count of converted files: " << dbgCount;
    emit scalingDone();
}

/*!
  Slot, that's being called when the QGalleryQueryRequest finishes with error.
*/
void ImageScaler::requestError()
{
    qDebug() << "ImageScaler::requestError";
    emit scalingDone();
}
// End of file
