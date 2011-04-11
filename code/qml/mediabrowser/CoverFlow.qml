import QtQuick 1.0
import QtMobility.gallery 1.1

Rectangle {
    id: container

    property DocumentGalleryModel model
    property int topMargin: 10
    property int bottomMargin: 20

    width: 640
    height: 360
    color: "steelblue"

    Component.onCompleted: {
        console.log("Model has " + model.count + " images");
    }

    PathView {
        id: pathView
        model: container.model
        delegate: coverFlowDelegate
        anchors.fill: parent
        path: coverFlowPath
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        focus: true
        interactive: true
    }

    Path {
        id: coverFlowPath
        startX: 0
        startY: coverFlow.height / 2
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: 60 }
        PathAttribute { name: "iconScale"; value: 0.3 }
        PathLine { x: coverFlow.width / 2; y: coverFlow.height / 2;  }
        PathAttribute { name: "z"; value: 100 }
        PathAttribute { name: "angle"; value: 0 }
        PathAttribute { name: "iconScale"; value: 1.0 }
        PathLine { x: coverFlow.width; y: coverFlow.height / 2; }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -60 }
        PathAttribute { name: "iconScale"; value: 0.3 }
    }

    Component {
        id: coverFlowDelegate
        Image {
            id: delegateImage
            y: container.topMargin
            x: 0
            width: height
            height: container.height - container.topMargin - container.bottomMargin
            z: PathView.z
            scale: PathView.iconScale
            source: url
            fillMode: Image.PreserveAspectFit
            asynchronous: true
            transform: Rotation {
                origin.x: delegateImage.width/2; origin.y: delegateImage.height/2
                axis.x: 0; axis.y: 1; axis.z: 0     // rotate around y-axis
                angle: PathView.angle
            }
        }
    }
}
