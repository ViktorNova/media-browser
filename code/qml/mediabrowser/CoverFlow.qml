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
    color: "steelblue"

    Component.onCompleted: {
        console.log("Model has " + model.count + " images");
    }

    PathView {
        id: pathView
        model: container.model
        delegate: CoverFlowDelegate {}
        anchors.fill: parent
        path: coverFlowPath
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        focus: true
        interactive: true
        smooth: true
        pathItemCount: 7
        onCurrentIndexChanged: container.currentIndexChanged(pathView.currentIndex);
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

}
