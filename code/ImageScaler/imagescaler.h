/**
 * Copyright (c) 2012 Nokia Corporation.
 */

#ifndef IMAGESCALER_H
#define IMAGESCALER_H

#include <QtCore/QObject>
#include <QDocumentGallery>
#include <QGalleryQueryRequest>

class QFileInfo;
class QImage;

QTM_USE_NAMESPACE

class ImageScaler: public QObject
{
    Q_OBJECT

public:
    ImageScaler(const int thumbSize, QObject* parent = 0);
    virtual ~ImageScaler();

public slots:
    void init();
    bool scaleImages();

signals:
    void scalingDone();

private slots:
    void requestFinished();
    void requestError();

private: // Methods
    bool convertToThumb(const QFileInfo& info);
    bool saveImage(const QFileInfo& info, const QString& saveName);
    void fetchImageResults();

private: // Data
    int mThumbSize;

    QDocumentGallery *m_documentGallery;    // Owned
    QGalleryQueryRequest* m_galleryQuery;   // Owned
};

#endif // IMAGESCALER_H
