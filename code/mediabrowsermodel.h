/**
 * Copyright (c) 2012 Nokia Corporation
 */

#ifndef MEDIABROWSERMODEL_H
#define MEDIABROWSERMODEL_H

#include <QObject>
#include <QUrl>
#include <QDebug>
#include <QDate>
#include <QAbstractListModel>

#include <QDocumentGallery>
#include <QGalleryQueryRequest>
QTM_USE_NAMESPACE

class MediaDataObject;

class MediaBrowserModel : public QAbstractListModel
{
    Q_OBJECT

    Q_PROPERTY(bool isReady READ isReady)

    enum MediaBrowserRoles {
        UrlRole = Qt::UserRole + 1,
        FilePathRole,
        DateTakenRole,
        CameraModelRole,
        WidthRole,
        HeightRole,
        FileSizeRole,
        IsVideoRole
    };

public:
    explicit MediaBrowserModel(QObject *parent = 0);
    ~MediaBrowserModel();

    bool isReady() const;
    void addMediaDataObject(MediaDataObject *mediaDataObject);

signals:
    void browserModelReady();

public slots:
    void fetchData();

protected:
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role) const;

private slots:
    void onVideoQueryFinished();
    void onVideoQueryError(int,QString);
    void onImageQueryFinished();
    void onImageQueryError(int,QString);

private:
    void signalIfModelReady();

private:
    QDocumentGallery *m_documentGallery;            // Owned
    QGalleryQueryRequest *m_videoGalleryRequest;    // Owned
    QGalleryQueryRequest *m_imageGalleryRequest;    // Owned
    QList<MediaDataObject*> m_modelData;            // Owned

    bool m_imagesReady;
    bool m_videosReady;
    bool m_isReady;
};

#endif // MEDIABROWSERMODEL_H
