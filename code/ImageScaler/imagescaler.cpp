// Qt includes
#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFileInfoList>

#include <QtGui/QImage>
#include <QtGui/QImageReader>

// Class declaration
#include "imagescaler.h"

// Constants
static const QString ThumbDirPostfix("thumbs");
// Default length for height or width of the thumbnail.
static const int DefaultThumbSize = 256;

ImageScaler::ImageScaler(QObject *parent):
    QObject(parent),
    mSaveDir(0),
    mThumbSize(0)
{
    qDebug() << "ImageScaler object constructed!";
}

ImageScaler::~ImageScaler()
{
    delete mSaveDir;
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

    // Create the "/thumbs" subfolder
    if (mSaveDir)
        delete mSaveDir;
    mSaveDir = new QDir(path+ThumbDirPostfix);
    if (!mSaveDir->exists()) {
        qDebug() << "Creating thumbs folder!";
        if (!imageDir.mkdir(ThumbDirPostfix)) {
            qDebug() << "Thumbs folder creation failed. Quitting!";
            return false;
        }
    }

    qDebug() << "Requested thumb size: " << thumbSize;
    qDebug() << "ImageDir " << imageDir.path() << " & SaveDir " << mSaveDir->path();

    // Read the contents of the dir
    QFileInfoList list = imageDir.entryInfoList();
    for (int i = 0; i < list.size(); ++i) {
        QFileInfo info = list.at(i);

        // Convert the image to a thumbnail
        if (info.isFile()) {
            convertToThumb(info.filePath());
        }

        // Uncomment, if you would like to see the contents of the dir
        //qDebug() << info.absoluteFilePath();
    }

    return true;
}

bool ImageScaler::convertToThumb(const QFileInfo& info)
{
    bool retVal = false;
    // TODO: Maybe use some DB solution to save the information?
    if (mSaveDir->exists()) {
        QString saveName = mSaveDir->path() + "/" + info.fileName();

        // Check if the file already exists
        if (QFile::exists(saveName)) {
            qDebug() << "File: " << saveName << " already exist!";
        } else {
            // Does not exist yet, read, scale & save the image!
            retVal = saveImage(info);
        }
    }
    else {
        qDebug() << "Savedir does not exist!";
    }

    return retVal;
}

bool ImageScaler::saveImage(const QFileInfo& info)
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
    QString saveName = mSaveDir->path() + "/" + info.fileName();
    retVal = thumbnail.save(saveName);

    qDebug() << "Image: " << info.fileName() << " saved to " << mSaveDir->path()
             << ", img.save()'s retVal was: " << retVal << "!";

    return retVal;
}

// End of file
