/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: coverFlow

    // Defines, how many items will be shown on the pathview concurrently.
    // Works also with 9 items, just slows down a bit.
    property int itemCount: 7
    // Defines, where the CoverFlowDelegate will be positioned on the Y-axis.
    property int topMargin: 80

    Component.onCompleted: {
        pathView.currentIndex = 0;
    }

    anchors.fill: parent

    PathView {
        id: pathView

        model: galleryModel
        delegate: CoverFlowDelegate {
            topMargin: 80
            fullImageWidth: coverFlow.width * 0.875
            fullImageHeight: coverFlow.height * 0.875
        }
        path: coverFlowPath
        pathItemCount: coverFlow.itemCount

        anchors.fill: parent
        preferredHighlightBegin: 0.5
        preferredHighlightEnd: 0.5
        focus: true
    }

    // The path defining where the items appear and how they move around.
    //
    // Note: The x-axis value is defined in percentages of the
    // screen width like below. The path is a straight horizontal line.
    Path {
        id: coverFlowPath

        // "Start zone"
        startX: -25
        startY: coverFlow.height / 2
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: 70 }
        PathAttribute { name: "iconScale"; value: 0.6 }

        // Just before middle
        PathLine { x: coverFlow.width * 0.35; y: coverFlow.height / 2; }
        PathAttribute { name: "z"; value: 50 }
        PathAttribute { name: "angle"; value: 45 }
        PathAttribute { name: "iconScale"; value: 0.85 }
        PathPercent { value: 0.40 }

        // Middle
        PathLine { x: coverFlow.width * 0.5; y: coverFlow.height / 2; }
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
        PathLine { x: coverFlow.width + 25; y: coverFlow.height / 2; }
        PathAttribute { name: "z"; value: 0 }
        PathAttribute { name: "angle"; value: -70 }
        PathAttribute { name: "iconScale"; value: 0.6 }
        PathPercent { value: 1.0 }
    }
}
