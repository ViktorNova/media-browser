/**
 * Copyright (c) 2012 Nokia Corporation.
 */

import QtQuick 1.1
import com.nokia.meego 1.0

// Provides some information about the image, like the path to the file &
// date taken & camera model etc.
Item {

    property int __textMaxWidth: height >= width ? (width - 10) / 2 : (width - 10) / 3
    property int __textMaxWidthContent: height >= width ? __textMaxWidth : __textMaxWidth * 2

    signal closed

    anchors.fill: parent

    Image {
        anchors.fill: parent
        source: "gfx/photo_back.png"
    }

    Button {
        id: closeButton

        width: 70
        height: 70
        iconSource: "gfx/close_stop.svg"
        anchors {
            top: parent.top
            right: parent.right
            margins: 10
        }

        // Signal closing the InfoView of the image.
        onClicked: parent.closed()
    }

    // Text Element pair, which shows the file path information.
    Text {
        id: fileInfo

        width: __textMaxWidth
        anchors {
            top: closeButton.bottom
            topMargin: 10
            left: parent.left
            leftMargin: 10
        }
        font {
            pixelSize: 22
            bold: true
        }
        wrapMode: Text.Wrap
        text: qsTr("Filepath:")
    }
    Text {
        id: fileInfoContent

        function __urlToLocalFile(url) {
            var urlString = url.toString();
            var prefix = "file://";
            if (urlString.indexOf(prefix) === 0) {
                return urlString.substring(prefix.length);
            } else {
                return urlString;
            }
        }

        width: __textMaxWidthContent
        anchors {
            top: fileInfo.top
            left: fileInfo.right
        }
        // Filepaths may be long. Limit the maximum lines.
        maximumLineCount: 4
        font.pixelSize: 22
        wrapMode: Text.Wrap
        text: __urlToLocalFile(url)
    }

    // Text Element pair, which shows the file size information.
    Text {
        id: sizeInfo

        width: __textMaxWidth
        anchors {
            top: fileInfoContent.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
        }
        font {
            pixelSize: 22
            bold: true
        }
        wrapMode: Text.Wrap
        text: qsTr("Shown in size:")
    }
    Text {
        width: __textMaxWidthContent
        anchors {
            top: sizeInfo.top
            left: sizeInfo.right
        }
        font.pixelSize: 22
        wrapMode: Text.Wrap
        text: parent.width + "x" + parent.height
    }

    // Text Element pair, which shows the image taken date information.
    Text {
        id: takenInfo

        property variant takenOn: dateTaken

        visible: takenOn == "Invalid Date" ? false : true
        width: __textMaxWidth
        anchors {
            top: sizeInfo.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
        }
        font {
            pixelSize: 22
            bold: true
        }
        wrapMode: Text.Wrap
        text: qsTr("Picture taken on:")
    }
    Text {
        id: takenInfoContent

        visible: takenInfo.visible
        width: __textMaxWidthContent
        anchors {
            top: takenInfo.top
            left: takenInfo.right
        }
        font.pixelSize: 22
        maximumLineCount: 3
        wrapMode: Text.Wrap
        text: Qt.formatDateTime(takenInfo.takenOn, "yyyy-MM-dd hh:mm:ss")
    }

    // Text Element pair, which shows the camera model information.
    Text {
        id: cameraInfo

        visible: cameraModel == "" ? false : true
        width: __textMaxWidth
        anchors {
            top: takenInfoContent.bottom
            topMargin: 5
            left: parent.left
            leftMargin: 10
        }
        font {
            pixelSize: 22
            bold: true
        }
        wrapMode: Text.Wrap
        text: qsTr("Camera model:")
    }
    Text {
        visible: cameraInfo.visible
        width: __textMaxWidthContent
        anchors {
            top: cameraInfo.top
            left: cameraInfo.right
        }
        font.pixelSize: 22
        maximumLineCount: 2
        wrapMode: Text.Wrap
        text: cameraModel
    }

    // Mirroring rotation applied beforehand, because the ImageView will
    // rotate itself 180 degrees to show the "background" (which is InfoView).
    transform: Rotation {
        id: flip

        origin.x: parent.width / 2
        origin.y: parent.height / 2
        axis { x: 0; y: 1; z: 0 } // Rotate around y-axis.
        angle: 180
    }
}
