import QtQuick 1.0

Rectangle {
    width: 640
    height: 360
    color: "lightsteelblue"

    CoverFlow {
        id: coverFlow
        anchors {
            fill: parent
            topMargin: 80
        }
    }

    Button {
        anchors {
            top: parent.top
            right: parent.right
            margins: 10
        }

        text: "X"
        fontBold: true

        width: 60
        height: 60

        onClicked: Qt.quit();
    }
}

