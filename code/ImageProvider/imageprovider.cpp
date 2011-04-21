#include "imageprovider.h"

#include <QtCore/QDebug>
#include <QtDeclarative/qdeclarative.h>

ImageProvider::ImageProvider():
        //QDeclarativeImageProvider(QDeclarativeImageProvider::Pixmap)  // Synchronous loading!
        QDeclarativeImageProvider(QDeclarativeImageProvider::Image)
{
    qDebug() << "Constructing ImageProvider";

    // setFlag(ItemHasNoContents, false);
}

ImageProvider::~ImageProvider()
{
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "requestImage - id: " << id;

    if (size) {
        qDebug() << "requestingImage: " << id << ", of size ("
                 << size->width() << "," << size->height() << "), of requestedSize ("
                 << requestedSize.width() << "," << requestedSize.height() << ")";
    }
    else {
        qDebug() << "requestingImage: " << id << ", of requestedSize ("
                 << requestedSize.width() << "," << requestedSize.height() << ")";
    }

    // TESTING TESTING TESTING
    // VKN TODO: Lataa ja palauta täällä bitmäppi!
    //const QString imgPath("/home/vkes/Temp/Images/Backgrounds/thumbs/" + id);
    const QString imgPath("E:\\Images\\Backgrounds\\thumbs\\" + id);
    QImage img(imgPath);
    return img;
}

/*
QPixmap ImageProvider::requestPixmap(const QString& id, QSize* size, const QSize& requestedSize)
{
    // VKN TODO: Lataa ja palauta täällä bitmäppi!
    qDebug() << "requestPixmap - id: " << id;

    // TESTING TESTING TESTING
    const QString imgPath("/Temp/Images/Backgrounds/thumbs/Forest.jpg");
    QPixmap pixmap(imgPath);
    return pixmap;
}
*/

// End of file
