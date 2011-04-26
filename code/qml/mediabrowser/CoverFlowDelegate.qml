import QtQuick 1.0
//import "../../ImageProvider/ImageProvider" // Import the ImageProvider
import ImageProvider 1.0 // Import the ImageProvider

Rectangle {
    id: delegateItem
    y: container.topMargin
    x: 0
    z: PathView.z
    width: delegateItem.height
    height: container.height - container.topMargin - container.bottomMargin
    scale: PathView.iconScale
    color: PathView.isCurrentItem ? Qt.lighter("blue") : "blue"
    radius: 10
    Image {
        clip: true
        id: delegateImage
        anchors.centerIn: parent
        width: delegateImage.height
        height: parent.height - 20

        // The image will be fetched from the imageprovider -plugin
        source: "image://imageprovider/" + url

//        source: {
//            if (pathView.still) {
//                //url;
//                // VKN
//                "image://imageprovider/Beach.jpg";
//            } else {
//                "gfx/image_placeholder.png";
//            }
//        }

        //Image.Ready
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        // Only set sourceSize.width or height to maintain aspect ratio
        sourceSize.width: delegateImage.width
    }
    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (pathView.currentIndex == index) {
                console.log("Clicked on current item, zoom it")
                //TODO real states with transitions and animations
                delegateItem.scale = 1.5
            } else {
                console.log("Clicked on item at index " + index)
                pathView.currentIndex = index;
            }
        }
    }

    transform: Rotation {
        origin.x: delegateImage.width/2; origin.y: delegateImage.height/2
        axis.x: 0; axis.y: 1; axis.z: 0     // rotate around y-axis
        angle: PathView.angle
    }
}
