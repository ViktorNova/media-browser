/**
 * Copyright (c) 2012 Nokia Corporation
 */

// Class declaration
#include "mediabrowsermodel.h"

// Internal includes
#include "mediadataobject.h"

// Includes
#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QGalleryQueryRequest>
#include <qgalleryproperty.h>

/*!
  \class MediaBrowserModel
  \brief This class provides model providing data from images and videos
         found on system.
*/

/*!
  Constructs a new media browser model.
*/
MediaBrowserModel::MediaBrowserModel(QObject *parent) :
    QAbstractListModel(parent),
    m_imagesReady(false),
    m_videosReady(false),
    m_isReady(false)
{
    // QML uses role names when accessing properties from model
    QHash<int, QByteArray> roles;
    roles[UrlRole] = "url";
    roles[FilePathRole] = "filePath";
    roles[DateTakenRole] = "dateTaken";
    roles[CameraModelRole] = "cameraModel";
    roles[WidthRole] = "width";
    roles[HeightRole] = "height";
    roles[FileSizeRole] = "fileSize";
    roles[IsVideoRole] = "isVideo";
    setRoleNames(roles);

    // QDocumentGallery for video fetching
    m_documentGallery = new QDocumentGallery(this);
    m_videoGalleryRequest = new QGalleryQueryRequest(m_documentGallery, this);
    m_videoGalleryRequest->setRootType("Video");
    connect(m_videoGalleryRequest, SIGNAL(finished()),
            this, SLOT(onVideoQueryFinished()));
    connect(m_videoGalleryRequest, SIGNAL(error(int,QString)),
            this, SLOT(onVideoQueryError(int,QString)));

    // QDocumentGallery for image fetching
    m_imageGalleryRequest = new QGalleryQueryRequest(m_documentGallery, this);
    m_imageGalleryRequest->setRootType("Image");
    connect(m_imageGalleryRequest, SIGNAL(finished()),
            this, SLOT(onImageQueryFinished()));
    connect(m_imageGalleryRequest, SIGNAL(error(int,QString)),
            this, SLOT(onImageQueryError(int,QString)));
}

/*!
   Destroys a media data object.
*/
MediaBrowserModel::~MediaBrowserModel()
{
    qDeleteAll(m_modelData);
}

/*!
   Whether the model is done fetching media information.
*/
bool MediaBrowserModel::isReady() const
{
    return m_isReady;
}

/*!
   Adds new media data object to model. Transfers ownership.
*/
void MediaBrowserModel::addMediaDataObject(MediaDataObject *mediaDataObject)
{
    if (mediaDataObject->url().toString().contains("/thumbs/")) {
        qDebug() << "Filtering thumbnail: " + mediaDataObject->url().toString();
        return;
    }

    beginInsertRows(QModelIndex(), rowCount(), rowCount());
    m_modelData.append(mediaDataObject);
    endInsertRows();
}

/*!
   Start fetching media data information
*/
void MediaBrowserModel::fetchData()
{
    qDeleteAll(m_modelData);
    m_modelData.clear();
    m_imagesReady = false;
    m_videosReady = false;
    m_isReady = false;

    if (!m_videoGalleryRequest->isSupported() ||
        !m_imageGalleryRequest->isSupported()) {
        qDebug() << "QGalleryQueryRequest not supported";

        m_isReady = true;
        emit browserModelReady();
        return;
    }

    m_videoGalleryRequest->execute();
    m_imageGalleryRequest->execute();
}

int MediaBrowserModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return m_modelData.count();
}

QVariant MediaBrowserModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() ||
            index.row() < 0 ||
            index.row() >= m_modelData.count()) {
        return QVariant();
    }

    MediaDataObject *mediaObject = m_modelData.at(index.row());

    if (role == UrlRole) {
        return mediaObject->url();
    } else if (role == FilePathRole) {
        return mediaObject->filePath();
    } else if (role == DateTakenRole) {
        return mediaObject->dateTaken();
    } else if (role == CameraModelRole) {
        return mediaObject->cameraModel();
    } else if (role == WidthRole) {
        return mediaObject->width();
    } else if (role == HeightRole) {
        return mediaObject->height();
    } else if (role == FileSizeRole) {
        return mediaObject->fileSize();
    } else if (role == IsVideoRole) {
        return mediaObject->isVideo();
    } else {
        qWarning() << "Unknown role: " << role;
    }
    return QVariant();
}

void MediaBrowserModel::onVideoQueryFinished()
{
    qDebug() << "Item count: " << m_videoGalleryRequest->itemCount();

    m_videoGalleryRequest->first();
    do {
        if (!m_videoGalleryRequest->isValid()) {
            qDebug() << "Item not valid";
            break;
        }        
        MediaDataObject *mediaData =
                new MediaDataObject(
                    m_videoGalleryRequest->itemUrl(),
                    m_videoGalleryRequest->metaData("filePath").toString(),
                    m_videoGalleryRequest->metaData("dateTaken").toDate(),
                    m_videoGalleryRequest->metaData("cameraModel").toString()
                    );

        mediaData->setWidth( m_videoGalleryRequest->metaData("width").toInt() );
        mediaData->setHeight( m_videoGalleryRequest->metaData("height").toInt() );
        mediaData->setFileSize(m_videoGalleryRequest->metaData("fileSize").toInt());
        mediaData->setIsVideo(true);
        addMediaDataObject(mediaData);
    } while (m_videoGalleryRequest->next());

    m_videosReady = true;
    signalIfModelReady();
}

void MediaBrowserModel::onVideoQueryError(int error, QString errorString)
{
    qDebug() << "MediaBrowserModel::videoQueryError";
    qDebug() << "Error: " << error;
    qDebug() << "errorString: " << errorString;

    m_videosReady = true;
    signalIfModelReady();
}

void MediaBrowserModel::onImageQueryFinished()
{
    qDebug() << "MediaBrowserModel::imageQueryFinished";
    qDebug() << "Item count: " << m_imageGalleryRequest->itemCount();
    m_imageGalleryRequest->first();

    do {
        if (!m_imageGalleryRequest->isValid()) {
            qDebug() << "Item not valid";
            break;
        }
        MediaDataObject *mediaData =
                new MediaDataObject(
                    m_imageGalleryRequest->itemUrl(),
                    m_imageGalleryRequest->metaData("filePath").toString(),
                    m_imageGalleryRequest->metaData("dateTaken").toDate(),
                    m_imageGalleryRequest->metaData("cameraModel").toString()
                    );
        addMediaDataObject(mediaData);
    } while (m_imageGalleryRequest->next());

    m_imagesReady = true;
    signalIfModelReady();
}

void MediaBrowserModel::onImageQueryError(int error, QString errorString)
{
    qDebug() << "MediaBrowserModel::imageQueryError";
    qDebug() << "Error: " << error;
    qDebug() << "errorString: " << errorString;

    m_imagesReady = true;
    signalIfModelReady();
}

void MediaBrowserModel::signalIfModelReady()
{
    // Video query in simulator does not give any results
    // or signaling so ignore it.
    bool skipVideos = false;
#ifdef QT_SIMULATOR
    qDebug() << "MediaBrowserModel::signalIfModelReady skip videos";
    skipVideos = true;
#endif // QT_SIMULATOR

    if (m_imagesReady && (m_videosReady || skipVideos)) {
        m_isReady = true;
        emit browserModelReady();
    }
}
