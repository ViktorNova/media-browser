import QtQuick 1.0
import ImageProvider 1.0 // Import the ImageProvider

Rectangle {
    id: delegateItem
    x: 0
    z: PathView.z
    width: delegateItem.height / 2
    height: container.height
    scale: PathView.iconScale
//    color: PathView.isCurrentItem ? Qt.lighter("blue") : "blue"
//    color: "black"
    color: "transparent"
    radius: 10

    // The coverflow item consists of two images with white borders.
    // The second image is flipped and has some opacity for nice mirror effect.
    Column  {
        id: delegate
        y: container.topMargin
        spacing: 5

        // White borders around the image
        Rectangle {
            id: delegateImage
            width: delegateItem.width
            height: delegateImage.width
            color: "white"

            // Should go on top of the reflection image when zooming.
            z: reflection.z + 1

            Image {
                id: dlgImg

                width: delegateItem.width - 8
                height: delegateImage.width - 8
                anchors.centerIn: parent

                // Only set sourceSize.width or height to maintain aspect ratio.
                sourceSize.width: delegateImage.width - 8
                clip: true

                // Don't stretch the image, and use asynchronous loading.
                fillMode: Image.PreserveAspectCrop
                asynchronous: true

                // The image will be fetched from the imageprovider -plugin.
                source: "image://imageprovider/" + url

                // Smoothing slows down the scrolling even more. Use it with consideration.
                //smooth: true
            }
        }

        // Reflection
        Item {
            width: delegateImage.width
            height: delegateImage.width

            Image {
                id: reflection

                width: delegateImage.width
                height: delegateImage.width
                anchors.centerIn: parent
                sourceSize.width: delegateImage.width
                clip: true

                // The reflection uses the same image as the delegateImage.
                // This way there's no need to ask the image again from the provider.
                //source: dlgImg.source
                source: "image://imageprovider/reflection/" + url

                fillMode: Image.PreserveAspectCrop
                asynchronous: true
                //smooth: true

                transform : Scale {
                    yScale: -1
                    origin.y: delegateImage.height / 2
                }

                // TODO: This should create a layer of opacity on top of the image!
                // Still needs some work.
                //AlphaGradient {}
            }
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (pathView.currentIndex == index) {
                console.log("Clicked on current item, zoom it")
                // TODO real states with transitions and animations
                //delegateItem.scale = 1.5
                parent.state == "scaled" ?
                    parent.state = "" : parent.state ="scaled"
            } else {
                console.log("Clicked on item at index " + index + ", focusing it")
                pathView.currentIndex = index;
            }
        }
    }

    // Rotation depends on the item's position on the PathView.
    // I.e. nicely rotate the image & reflection around Y-axis before disappearing.
    transform: Rotation {
        origin.x: delegateImage.width / 2
        origin.y: delegateImage.height / 2
        axis { x: 0; y: 1; z: 0 } // Rotate around y-axis.
        angle: PathView.angle
    }

    // States and transitions for scaling the image.
    states: [
        State {
            name: "scaled"
            PropertyChanges {
                target: delegateImage
                clip: false
                // Scale up the icon
                scale: 1.5
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "scaled"
            reversible: true
            PropertyAnimation {
                target: delegateImage
                properties: "scale"
                duration: 300
            }
        }
    ]
}
