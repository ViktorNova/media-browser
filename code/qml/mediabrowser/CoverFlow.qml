import QtQuick 1.0
import QtMobility.gallery 1.1

Rectangle {
    id: container

    signal currentIndexChanged(int index)

    property DocumentGalleryModel model
    property int topMargin: 80
    property int bottomMargin: 80

    width: 640
    height: 360
    //color: "steelblue"
    //color: "black"
    // Background image
    Image {
        id: background

        width: parent.width
        height: parent.height
        source: "gfx/background_n8.png"
    }

    Component.onCompleted: {
        console.log("Model has " + model.count + " images");
        pathView.currentIndex = 0;
    }

    PathView {
        id: pathView
        property bool still: true

        model: container.model
        delegate: CoverFlowDelegate {}
        anchors.fill: parent
        path: coverFlowPath
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        focus: true
        interactive: true
        pathItemCount: 7
        //pathItemCount: 9  // Works also with 9 items, just slows down a bit.

        onMovementStarted: {
            pathView.still = false;
            console.log("PathView movement started")
        }

        onMovementEnded: {
            pathView.still = true;
            console.log("PathView movement ended")
        }

        Keys.onRightPressed: {
            if (interactive) { // && !moving) {
                incrementCurrentIndex()
            }
        }

        Keys.onLeftPressed: {
            if (interactive) { // && !moving) {
                decrementCurrentIndex()
            }
        }

        onCurrentIndexChanged: {
            container.currentIndexChanged(pathView.currentIndex);
        }
    }

    // The path defining where the items appear and how they move around.
    //
    // Note: The x-axis value is defined in percentages of the
    // screen width like below. The path is a straight horizontal line.
    Path {
        id: coverFlowPath

        // "Start zone"
        startX: 0
        startY: coverFlow.height / 2
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: 70 }
        PathAttribute { name: "iconScale"; value: 0.6 }

        // Just before middle
        PathLine { x: coverFlow.width * 0.35; y: coverFlow.height / 2;  }
        PathAttribute { name: "z"; value: 50 }
        PathAttribute { name: "angle"; value: 45 }
        PathAttribute { name: "iconScale"; value: 0.85 }
        PathPercent { value: 0.40 }

        // Middle
        PathLine { x: coverFlow.width * 0.5; y: coverFlow.height / 2;  }
        PathAttribute { name: "z"; value: 100 }
        PathAttribute { name: "angle"; value: 0 }
        PathAttribute { name: "iconScale"; value: 1.0 }

        // Just after middle
        PathLine { x: coverFlow.width * 0.65; y: coverFlow.height / 2; }
        PathAttribute { name: "z"; value: 50 }
        PathAttribute { name: "angle"; value: -45 }
        PathAttribute { name: "iconScale"; value: 0.85 }
        PathPercent { value: 0.60 }

        // Final stop
        PathLine { x: coverFlow.width; y: coverFlow.height / 2; }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -70 }
        PathAttribute { name: "iconScale"; value: 0.6 }
        PathPercent { value: 1.0 }
    }

}
