import QtQuick 1.0

Item {
    id: imageView

    // The width and height will be defined by the size
    // of the shown image!
    width: img.width
    height: img.height

    // These define, how big can the shown image be at maximum.
    property int maxWidth: 560
    property int maxHeight: 340

    property string imagePath: ""
    property int fillMode: Image.PreserveAspectFit

    signal closed();

    // "Closes" (hides) the ImageView and signals that the ImageView
    // has been hidden.
    function close() {
        console.log("ImageView::close()")
        imageView.state = "";
        imageView.closed();
    }

    opacity: 0

    Image {
        id: img
        fillMode: Image.PreserveAspectFit
        anchors.centerIn: parent

        // Image size will be automatic, determined by the imageprovider!
        //width: imageView.width
        //height: imageView.height

        sourceSize.width: imageView.maxWidth
        sourceSize.height: imageView.maxHeight
        smooth: true

        // Setting this will provide a larger image
        source: "image://imageprovider/full/" + imagePath
    }

    MouseArea {
        // A full screen mousearea to capture clicks outside the image. If
        // clicked outside, then close the ImageView.
        x: -200
        y: -200
        width: 840
        height: 560
        onClicked: {
            console.log("Full screen mousearea clicked. Closing ImageView.")
            imageView.close()
        }
    }

    MouseArea {
        // This one will prevent the touch events to close the image if clicked
        // on the image itself.
        anchors.fill: parent
        onClicked: {
            console.log("capturing touch events on imageView")
        }
    }

    Button {
        anchors {
            top: img.top
            right: img.right
            margins: 35
        }

        buttonName: "zoomButton"
        bgImage: "gfx/zoom_out.png"
        bgImagePressed: "gfx/zoom_out.png"

        width: 49
        height: 49
        opacity: 0.8

        onClicked: {
            console.log("zoomButton onClicked");
            imageView.close();
        }
    }

    Button {
        anchors {
            bottom: img.bottom
            right: img.right
            margins: 35
        }

        // TODO: Change final graphics in place, once available!
        buttonName: "infoButton"
        bgImage: "gfx/info.png"
        bgImagePressed: "gfx/info.png"

        width: 49
        height: 49
        opacity: 0.8

        onClicked: {
            console.log("infoButton clicked");
            parent.state = "info";
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
        }
    ]
}
