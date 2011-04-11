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
//            topMargin: 60
        }

        model: galleryModel
    }

    Button {
        anchors {
            top: parent.top
            right: parent.right
            margins: 10
        }

        buttonName: "exitButton"
        text: ""
        bgImage: "gfx/exit_button.png"
        bgImagePressed: "gfx/exit_button_pressed.png"

        width: 66
        height: 66

        onClicked: Qt.quit();
    }
}

