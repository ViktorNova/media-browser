#ifndef IMAGESCALER_H
#define IMAGESCALER_H

#include <QtCore/QObject>

#include "ImageScaler_global.h"

// Forward declarations
class QFileInfo;
class QImage;

class IMAGESCALERSHARED_EXPORT ImageScaler: public QObject
{
    Q_OBJECT

public:
    /*!
     * Constructs the ImageScaler. The path and thumbSize HAVE TO BE given.
     * @param path Path to the folder in which the images should be converted to thumbnails.
     * @param thumbSize The maximum length that the longer dimension (width/height) will receive.
     */
    ImageScaler(const QString& path, const int thumbSize, QObject* parent = 0);
    virtual ~ImageScaler();

public slots:


    /*!
     * Method to convert the images to thumbs in the specified folder.
     * Scales the images within the given path to thumbnails into "/thumbs" subfolder.
     */
    bool scaleImages();

private:

    // Worker methods, that do the actual conversion & saving
    bool convertToThumb(const QFileInfo& info);
    bool saveImage(const QFileInfo& info, const QString& saveName);

private: // Data
    QString mPath;
    int mThumbSize;
};

#endif // IMAGESCALER_H
