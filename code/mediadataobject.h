/**
 * Copyright (c) 2012 Nokia Corporation
 */

#ifndef MEDIADATAOBJECT_H
#define MEDIADATAOBJECT_H

#include <QObject>
#include <QString>
#include <QUrl>
#include <QDate>

class MediaDataObject : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QUrl url READ url WRITE setUrl)
    Q_PROPERTY(QString filePath READ filePath WRITE setFilePath)
    Q_PROPERTY(QDate dateTaken READ dateTaken WRITE setDateTaken)
    Q_PROPERTY(QString cameraModel READ cameraModel WRITE setCameraModel)
    Q_PROPERTY(int fileSize READ fileSize WRITE setFileSize)
    Q_PROPERTY(bool isVideo READ isVideo WRITE setIsVideo)

public:
    MediaDataObject(QUrl url,
                    QString filePath,
                    QDate dateTaken,
                    QString cameraModel,
                    QObject *parent = 0);

    void setUrl(QUrl url);
    QUrl url() const;

    void setFilePath(QString filePath);
    QString filePath() const;

    void setDateTaken(QDate dateTaken);
    QDate dateTaken() const;

    QString cameraModel() const;
    void setCameraModel(QString cameraModel);

    int width() const;
    void setWidth(int width);

    int height() const;
    void setHeight(int height);

    int fileSize() const;
    void setFileSize(int fileSize);

    int isVideo() const;
    void setIsVideo(bool isVideo);

private:
    QUrl m_url;
    QString m_filePath;
    QDate m_dateTaken;
    QString m_cameraModel;
    int m_width;
    int m_height;
    int m_fileSize;
    bool m_isVideo;
};

#endif // MEDIADATAOBJECT_H
