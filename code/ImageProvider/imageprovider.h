/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QtDeclarative/QDeclarativeItem>
#include <QtDeclarative/QDeclarativeImageProvider>

class QImage;

class ImageProvider: public QDeclarativeImageProvider
{
    Q_DISABLE_COPY(ImageProvider)

public:
    ImageProvider();
    ~ImageProvider();

    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);

private: // Methods
    QImage getImage(const QString& path, QSize *size, const QSize& requestedSize,
                    bool fullImage = false);
    void maskImage(QImage& orig);

private: // Data
    // For caching images internally
    QHash<QString, QImage> mCache;

    // For reflection
    QImage mFadeMask;

    // For returning static video thumbnail quickly
    QImage mVideoThumb;
    QImage mVideoFull;
    QImage mVideoThumbReflection;

    // Comparison string members
    const QString mImageThumbStr;
    const QString mImageFullStr;
    const QString mImageReflectionStr;
    const QString mVideoThumbStr;
    const QString mVideoFullStr;
    const QString mVideoReflectionStr;
};

#endif // IMAGEPROVIDER_H
