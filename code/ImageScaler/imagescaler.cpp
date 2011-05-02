// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QDirIterator>
#include <QtCore/QFileInfoList>

#include <QtGui/QImage>
#include <QtGui/QImageReader>

// Class declaration
#include "imagescaler.h"

// Constants
#define THUMBS_DIR "thumbs"
#define THUMBS_DIR_WITH_PREFIX "/" THUMBS_DIR

//static const QString ThumbDirPostfix("thumbs");
// Default length for height or width of the thumbnail.
static const int DefaultThumbSize = 256;

ImageScaler::ImageScaler(QObject *parent):
    QObject(parent),
    mThumbSize(0)
{
    qDebug() << "ImageScaler object constructed!";
}

ImageScaler::~ImageScaler()
{
//    delete mSaveDir;
}

bool ImageScaler::scaleImages(const QString& path, int thumbSize)
{
    // Save the thumbdimension size for use. Though do some error checing first.
    mThumbSize = thumbSize;
    if (mThumbSize < 0) {
        mThumbSize = DefaultThumbSize;
    }

    // Check that the directory from which to read really exists.
    QDir imageDir(path);
    if (!imageDir.exists()) {
        qDebug() << "Cannot find the imagedir (" << imageDir.absolutePath() << "). Quitting!";
        return false;
    }

    qDebug() << "Requested thumb size: " << thumbSize;
    qDebug() << "ImageDir " << imageDir.path();
    // Debug counting of files
    int dbgCount = 0;

    // Iterate through all the files also in the subfolders
    QDirIterator dirIter(imageDir, QDirIterator::Subdirectories);
    while (dirIter.hasNext()) {
        // Uncomment, if you would like to see the contents of the dir
        qDebug() << dirIter.filePath();

        // Fetch the QFileInfo from the current dir iterator
        QFileInfo info = dirIter.fileInfo();

        // Convert the image to a thumbnail, if it is a file and the folder
        // in which the file exists isn't a /thumbs -folder!
        if (info.isFile() && info.filePath().indexOf(THUMBS_DIR_WITH_PREFIX) < 0) {
            // Convert to thumbnail
            convertToThumb(info);
            dbgCount++;
        }
        else {
            // item was either a folder itself or a file which was in
            // a thumbs folder. Just debug print here.
            qDebug() << "Skipped " << dirIter.filePath();
        }

        // Move the iterator forward
        dirIter.next();
    }

    // DEBUG PRINT! Uncomment, if not needed.
    qDebug() << "Count of converted files: " << dbgCount;

    return true;
}

bool ImageScaler::convertToThumb(const QFileInfo& info)
{
    bool retVal = false;

    // Create the "/thumbs" subfolder, if not already exist!
    QDir saveDir(info.path()+ THUMBS_DIR_WITH_PREFIX);
    if (!saveDir.exists()) {
        qDebug() << "Creating thumbs folder!";
        QDir topDir(info.path());
        if (!topDir.mkdir(THUMBS_DIR)) {
            qDebug() << "Thumbs folder creation failed. Quitting!";
            return false;
        }
    }

    // TODO: Maybe use some DB solution to save the information?
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

bool ImageScaler::saveImage(const QFileInfo& info, const QString& saveName)
{
    bool retVal = false;
    // Create the image reader to handle the downscaling of the image
    QImageReader imageReader(info.filePath());
    if (!imageReader.canRead()) {
        qDebug() << "ERROR: ImageReader cannot read file from: " << info.filePath() << "!";
        return retVal;
    }

    // Calculate the new dimensions for the thumbnail. Preserve aspect ratio.
    int width = imageReader.size().width();
    int height = imageReader.size().height();

    if (width > height) {
        height = static_cast<double>(mThumbSize) / width * height;
        width = mThumbSize;
    } else if (width < height) {
        width = static_cast<double>(mThumbSize) / height * width;
        height = mThumbSize;
    } else {
        width = mThumbSize;
        height = mThumbSize;
    }

    // Set the correct size to which we want the image to be read. Read it.
    imageReader.setScaledSize(QSize(width, height));
    QImage thumbnail = imageReader.read();
    // Save the image to disk right away
    retVal = thumbnail.save(saveName);

    qDebug() << "Image: " << info.fileName() << " saved to " << saveName
             << ", img.save()'s retVal was: " << retVal << "!";

    return retVal;
}

// End of file
