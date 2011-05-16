import QtQuick 1.0
import QtMobility.gallery 1.1

Rectangle {
    width: 640
    height: 360
    color: "lightsteelblue"
    property bool loading: false
    property bool loadingComplete: false

    WaitIndicator {
        id: waitIndicator
        anchors.fill: parent
        delay: 0
        z: 120
        show: true
    }

    DocumentGalleryModel {
        id: galleryModel
        rootType: DocumentGallery.Image
        scope: DocumentGallery.Image
        properties: [ "url" ]
        limit: 10
        autoUpdate: true
        onProgressChanged: {
            console.log("Model progress: " + progress)
            // TODO: Use nice constants instead of these magic params!
            if (!loadingComplete) {
                if (!loading && progress == 0) {
                    console.log("Starting image scaling process & WaitIndicator from main.qml")
                    loading = true
                    waitIndicator.show = true
                    cpThumbCreator.scaleImages("E:\\Images\\", 180)
                }
                else if (loading && progress == 1) {
                    console.log("Loading complete!")
                    loading = false
                    loadingComplete = true
                    waitIndicator.show = false
                }
                else {
                    console.log("Just wait.")
                }
            }
        }
        onStatusChanged: console.log("Model status: " + status)
    }

    CoverFlow {
        id: coverFlow
        anchors {
            fill: parent
        }

        model: galleryModel

        onCurrentIndexChanged: console.log("Current index: " + index)
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

        width: 64
        height: 64

        onClicked: Qt.quit();
    }
}

