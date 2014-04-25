/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: imageView

    // Set for to differentiate the image & video thumbnails.
    property bool isVideo: false

    // These are needed for VideoStreamer integration.
    property bool showToolBar: false
    property bool showStatusBar: false

    // These define, how big can the shown image be at maximum.
    property int maxWidth: 560
    property int maxHeight: 340

    property string imagePath: ""
    property int fillMode: Image.PreserveAspectFit

    signal closed

    // "Closes" (hides) the ImageView and signals that the ImageView
    // has been hidden.
    function close() {
        imageView.state = "";
        imageView.closed();
    }

    // The width and height will be defined by the size
    // of the shown image!
    width: img.width
    height: img.height
    opacity: 0

    Image {
        id: img

        function __urlToLocalFile(url) {
            var urlString = new String(url);
            var prefix = "file://";
            if (urlString.indexOf(prefix) === 0) {
                return urlString.substring(prefix.length);
            } else {
                return urlString;
            }
        }

        // Function, that determines should the large image be either image
        // or a generic video information image.
        function __getImagePath() {
            var prefix = "";
            if (imageView.imagePath) {
                if (imageView.isVideo) {
                    prefix = "image://imageprovider/videoFull/";
                } else {
                    prefix = "image://imageprovider/imageFull/";
                }
            }

            return prefix + imageView.imagePath;
        }

        // Image size will be automatic, determined by the imageprovider!
        //width: imageView.width
        //height: imageView.height
        sourceSize.width: imageView.maxWidth
        sourceSize.height: imageView.maxHeight
        source: __getImagePath()

        anchors.centerIn: parent
        fillMode: Image.PreserveAspectFit
        smooth: true

        Column {
            visible: imageView.isVideo
            spacing: 3

            anchors {
                top: parent.verticalCenter
                left: parent.left
                right: parent.right
                bottom: parent.bottom
                leftMargin: 20
                rightMargin: 20
                topMargin: img.height / 4
            }

            Text {
                color: "white"
                width: parent.width
                text: img.__urlToLocalFile(model.url)
                wrapMode: Text.WrapAnywhere
                maximumLineCount: 1
                font.pixelSize: 22
            }

            Text {
                color: "white"
                width: parent.width
                visible: model.width !== 0 && model.height !== 0
                text: "Shown in size: " + model.width + " x " + model.height
                wrapMode: Text.WrapAnywhere
                maximumLineCount: 1
                font.pixelSize: 22
            }

            Text {
                color: "white"
                width: parent.width
                visible: model.fileSize !== 0
                text: "File size: " + ((model.fileSize) / 1048576).toFixed(2) + " MB";
                wrapMode: Text.WrapAnywhere
                maximumLineCount: 1
                font.pixelSize: 22
            }
        }
    }

    MouseArea {
        // A full screen mousearea to capture clicks outside the image. If
        // clicked outside, then close the ImageView.
        x: -200
        y: -200
        width: 840
        height: 560
        onClicked: {
            imageView.close()
        }
    }

    MouseArea {
        // This one will prevent the touch events to close the image if clicked
        // on the image itself.
        anchors.fill: parent
        onClicked: {
            console.log("Capturing touch events on imageView")
        }
    }

    // ZoomButton
    Button {
        width: 70
        height: 70
        iconSource: "gfx/close_stop.svg"
        anchors {
            top: img.top
            right: img.right
            margins: 10
        }

        onClicked: {
            imageView.close();
        }
    }

    // InfoButton
    Button {
        width: 70
        height: 70
        visible: !imageView.isVideo
        iconSource: "gfx/information_userguide.svg"
        anchors {
            bottom: img.bottom
            right: img.right
            margins: 10
        }

        onClicked: {
            parent.state = "info";
        }
    }

    // Button for starting video playback.
    Button {
        width: 70
        height: 70
        visible: imageView.isVideo
        anchors.centerIn: img
        iconSource: "image://theme/icon-m-toolbar-mediacontrol-play"

        onClicked: {
            var component = Qt.createComponent("VideoPlayer/VideoPlayView.qml");

            if (component.status === Component.Ready) {
                console.log("Video player creation success");

                // Hide CoverFlow while playing video. This helps
                // reducing memory usage and allows smooth video playback.
                coverFlow.visible = false

                // Create the VideoPlayer & connect an exit handling function
                // to Player's exit signal. I.e. show CoverFlow & delete player.
                var player = component.createObject(mainView);
                function exitHandler() {
                    coverFlow.visible = true;
                    player.destroy();
                }
                player.videoExit.connect(exitHandler);

                // Set the video to be full screen and seekable.
                player.isFullScreen = true;
                player.enableScrubbing = true;

                // Create a data model object. (No need to set the video
                // extra descriptions, as showing videos always fullscreen).
                var model = new Object();
                model.m_contentUrl = url;
                player.setVideoData(model);
            }
        }
    }

    InfoView {
        id: infoView

        height: parent.height
        width: parent.width
        // Only show the infoview when the image has been "flipped"
        visible: flipAngle.angle > 90 ? true : false
        // Return back to visible state
        onClosed: parent.state = "visible"
    }

    transform: Rotation {
        id: flipAngle

        origin.x: img.width / 2
        origin.y: img.height / 2
        axis { x: 0; y: 1; z: 0 } // Rotate around y-axis.
        angle: 0
    }

    states: [
        State {
            name: "visible"
            PropertyChanges {
                target: imageView
                opacity: 1.0
            }
        },
        State {
            name: "info"
            PropertyChanges {
                target: flipAngle
                angle: 180
            }
            PropertyChanges {
                target: imageView
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "visible"

            PropertyAnimation {
                target: imageView
                properties: "opacity"
                duration: 500
            }
        },
        Transition {
            from: "visible"
            to: ""

            PropertyAnimation {
                target: imageView
                properties: "opacity"
                duration: 200
            }
        },
        Transition {
            from: "visible"
            to: "info"
            reversible: true

            PropertyAnimation {
                target: flipAngle
                properties: "angle"
                duration: 300
            }
        },
        Transition {
            from: "info"
            to: ""

            SequentialAnimation {
                PropertyAnimation {
                    target: imageView
                    properties: "opacity"
                    duration: 200
                }
                PropertyAction { target: flipAngle; property: "angle"}
            }
        }
    ]
}
