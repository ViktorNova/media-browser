import QtQuick 1.0

Rectangle {
    width: 640
    height: 360
    color: "lightsteelblue"


    CoverFlow {
        id: coverFlow
        anchors {
            fill: parent
            topMargin: 40
        }
    }
}

