import QtQuick 1.0
import QtMobility.gallery 1.1

Rectangle {
    width: 640
    height: 360
    color: "lightsteelblue"

    DocumentGalleryModel {
        id: galleryModel
        rootType: DocumentGallery.Image
        properties: [ "url" ]
        limit: 30
        //filter: filter
    }

    CoverFlow {
        id: coverFlow
        anchors {
            fill: parent
            topMargin: 60
        }

        model: galleryModel
    }

    Button {
        anchors {
            top: parent.top
            right: parent.right
            margins: 10
        }

        text: "X"
        fontBold: true

        width: 40
        height: 40

        onClicked: Qt.quit();
    }
}

