import QtQuick 1.0
import "ImageProvider" // Import the ImageProvider

Rectangle {
    id: container

    width: 360
    height: 360
    color: "lightblue"

    Text {
        text: "Hello World"
        anchors.horizontalCenter: container.horizontalCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            Qt.quit();
        }
    }

    Image {
        id: pic

        width: 128
        height: 128
        anchors.centerIn: container

        // If the sourceSize is defined, it will be passed also to the imageprovider!
        sourceSize.width: 128
        sourceSize.height: 128

        source: "image://imageprovider/Pebbles.jpg"
        //source: "image://imageprovider/Forest.jpg"
        //source: "image://imageprovider/GrassandSky.jpg"
    }
}
