/**
 * Copyright (c) 2012 Nokia Corporation
 */

// Class declaration
#include "mediadataobject.h"

// Includes
#include <QtCore/QDebug>

/*!
  \class MediaDataObject
  \brief This class provides support for storing media data
         information from single image or video.
*/

/*!
   Constructs a new media data object.
*/
MediaDataObject::MediaDataObject(QUrl url,
                QString filePath,
                QDate dateTaken,
                QString cameraModel,
                QObject *parent)
    : QObject(parent),
      m_url(url),
      m_filePath(filePath),
      m_dateTaken(dateTaken),
      m_cameraModel(cameraModel),
      m_width(0),
      m_height(0),
      m_fileSize(0),
      m_isVideo(false)
{
}

/*!
  \property MediaDataObject::url
  \brief the URL of media item.
*/

void MediaDataObject::setUrl(QUrl url)
{
    m_url = url;
}

QUrl MediaDataObject::url() const
{
    return m_url;
}

/*!
  \property MediaDataObject::filePath
  \brief the filepath of media item.
*/

void MediaDataObject::setFilePath(QString filePath)
{
    m_filePath = filePath;
}

QString MediaDataObject::filePath() const
{
    return m_filePath;
}

/*!
  \property MediaDataObject::dateTaken
  \brief the date when media item was taken.
*/

void MediaDataObject::setDateTaken(QDate dateTaken)
{
    m_dateTaken = dateTaken;
}

QDate MediaDataObject::dateTaken() const
{
    return m_dateTaken;
}

/*!
  \property MediaDataObject::cameraModel
  \brief the camera model which was used to capture media item.
*/

QString MediaDataObject::cameraModel() const
{
    return m_cameraModel;
}

void MediaDataObject::setCameraModel(QString cameraModel)
{
    m_cameraModel = cameraModel;
}

/*!
  \property MediaDataObject::width
  \brief the width of media item.
*/

int MediaDataObject::width() const
{
    return m_width;
}

void MediaDataObject::setWidth(int width)
{
    m_width = width;
}

/*!
  \property MediaDataObject::height
  \brief the height of media item.
*/

int MediaDataObject::height() const
{
    return m_height;
}

void MediaDataObject::setHeight(int height)
{
    m_height = height;
}

/*!
  \property MediaDataObject::fileSize
  \brief the file size of media item.
*/

int MediaDataObject::fileSize() const
{
    return m_fileSize;
}

void MediaDataObject::setFileSize(int fileSize)
{
    m_fileSize = fileSize;
}

/*!
  \property MediaDataObject::isVideo
  \brief the information if media item is video.
*/

int MediaDataObject::isVideo() const
{
    return m_isVideo;
}

void MediaDataObject::setIsVideo(bool isVideo)
{
    m_isVideo = isVideo;
}
