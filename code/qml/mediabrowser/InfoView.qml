import QtQuick 1.0

// Provides some information about the image, like the path to the file &
// date taken & camera model etc.
Item {
    width: 340
    height: 340

    signal closed()
    property int textMaxWidth: width - 10

    Image {
        // Tiled "MediaBrowser" -photo background would be nice, but it
        // doesn't rotate with 3D-transformations for some reason :-/
//        fillMode: Image.Tile
        anchors.fill: parent
        source: "gfx/photo_back.png"
    }

    Text {
        id: fileInfo

        width: textMaxWidth
        anchors {
            top: parent.top
            topMargin: 10
            left: parent.left
            leftMargin: 10
        }

        wrapMode: Text.Wrap
        text: filePath
    }

    Text {
        id: sizeInfo

        width: textMaxWidth
        anchors {
            top: fileInfo.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
        }

        wrapMode: Text.Wrap
        text: "Shown in size: " + parent.width + "x" + parent.height
    }

    Text {
        id: takenInfo
        visible: dateTaken == "Invalid Date" ? false : true

        width: textMaxWidth
        anchors {
            top: sizeInfo.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
        }

        wrapMode: Text.Wrap
        text: "Picture taken on: " + dateTaken
    }

    Text {
        id: cameraInfo

        visible: cameraModel == "" ? false : true
        width: textMaxWidth
        anchors {
            top: takenInfo.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
        }

        wrapMode: Text.Wrap
        text: "Camera model: " + cameraModel
    }

    Button {
        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 30
        }

        buttonName: "infoButton"
        bgImage: "gfx/exit_button.png"
        bgImagePressed: "gfx/exit_button_pressed.png"

        width: 66
        height: 66
        opacity: 0.6

        onClicked: {
            console.log("infoView closebutton clicked")
            parent.closed()
        }
    }

    // Mirroring rotation applied beforehand, because the ImageView will
    // rotate itself 180 degrees to show the "background" (which is InfoView)
    transform: Rotation {
        id: flip
        origin.x: parent.width / 2
        origin.y: parent.height / 2
        axis { x: 0; y: 1; z: 0 } // Rotate around y-axis.
        angle: 180
    }
}
