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
        smooth: true
        pathItemCount: 7
   }

    Path {
        id: coverFlowPath
        startX: 0
        startY: coverFlow.height / 2
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: 90 }
        PathAttribute { name: "iconScale"; value: 0.4 }

        // First stop
        PathLine { x: coverFlow.width / 8; y: coverFlow.height / 2;  }
        PathAttribute { name: "z"; value: 50 }
        PathAttribute { name: "angle"; value: 45 }
        PathAttribute { name: "iconScale"; value: 0.8 }

        // Middle
        PathLine { x: coverFlow.width / 2; y: coverFlow.height / 2;  }
        PathAttribute { name: "z"; value: 100 }
        PathAttribute { name: "angle"; value: 0 }
        PathAttribute { name: "iconScale"; value: 1.0 }

        PathLine { x: coverFlow.width/8*7; y: coverFlow.height / 2; }
        PathAttribute { name: "z"; value: 50 }
        PathAttribute { name: "angle"; value: -45 }
        PathAttribute { name: "iconScale"; value: 0.8 }

        PathLine { x: coverFlow.width; y: coverFlow.height / 2; }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -90 }
        PathAttribute { name: "iconScale"; value: 0.4 }
    }

    Component {
        id: coverFlowDelegate
        Rectangle {
            id: delegateItem
            y: container.topMargin
            x: 0
            z: PathView.z
            width: 200
            height: container.height - container.topMargin - container.bottomMargin
            scale: PathView.iconScale
            color: "blue"
            radius: 10
            Image {
                anchors.centerIn: parent
                width: parent.width - 20
                height: parent.height - 20
                id: delegateImage
                source: url
                fillMode: Image.PreserveAspectFit
                asynchronous: true
                // Only set sourceSize.width or height to maintain aspect ratio
                sourceSize.width: width
            }
            transform: Rotation {
                origin.x: delegateImage.width/2; origin.y: delegateImage.height/2
                axis.x: 0; axis.y: 1; axis.z: 0     // rotate around y-axis
                angle: PathView.angle
            }
        }
    }
}
