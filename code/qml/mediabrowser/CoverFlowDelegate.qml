import QtQuick 1.0
//import "../../ImageProvider/ImageProvider" // Import the ImageProvider
import ImageProvider 1.0 // Import the ImageProvider

Rectangle {
    id: delegateItem
    x: 0
    z: PathView.z
    width: delegateItem.height / 2
    height: container.height
    scale: PathView.iconScale
    color: "black"
//    color: PathView.isCurrentItem ? Qt.lighter("blue") : "blue"
    radius: 10

    Column  {
        id: delegate
        y: container.topMargin
        spacing: 5

        Image {
            id: delegateImage

            width: delegateItem.width
            height: delegateImage.width
            // Only set sourceSize.width or height to maintain aspect ratio.
            sourceSize.width: delegateImage.width
            clip: true

            // Don't stretch the image, and use asynchronous loading.
            fillMode: Image.PreserveAspectCrop
            asynchronous: true

            // The image will be fetched from the imageprovider -plugin.
            source: "image://imageprovider/" + url

            // Smoothing slows down the scrolling even more. Use it with consideration.
            //smooth: true
        }

        // Reflection
        Image {
            id: reflection

            width: delegateImage.width
            height: delegateImage.width
            sourceSize.width: delegateImage.width
            clip: true

            // The reflection uses the same image as the delegateImage.
            // This way there's no need to ask the image again from the provider.
            source: delegateImage.source

            fillMode: Image.PreserveAspectCrop
            asynchronous: true
            //smooth: true

            transform : Scale {
                yScale: -1
                origin.y: delegateImage.height / 2
            }

            // TODO: This should create a layer of opacity on top of the image!
            // Still needs some work.
//            Rectangle {
//                anchors.fill: parent
//                gradient: Gradient {
//                    GradientStop { position: 0.0; color: Qt.rgba(0,0,0,1.0) }
//                    GradientStop { position: 0.7; color: Qt.rgba(0,0,0,0.8) }
//                    GradientStop { position: 1.0; color: Qt.rgba(0,0,0,0.2) }
//                }
//            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (pathView.currentIndex == index) {
                console.log("Clicked on current item, zoom it")
                // TODO real states with transitions and animations
                delegateItem.scale = 1.5
            } else {
                console.log("Clicked on item at index " + index)
                pathView.currentIndex = index;
            }
        }
    }

    transform: Rotation {
        origin.x: delegateImage.width / 2
        origin.y: delegateImage.height / 2
        axis { x: 0; y: 1; z: 0 } // rotate around y-axis
        angle: PathView.angle
    }
}
