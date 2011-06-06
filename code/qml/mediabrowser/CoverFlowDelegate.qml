import QtQuick 1.0
import ImageProvider 1.0 // Import the ImageProvider

Item {
    id: delegateItem
    x: 0
    z: PathView.z
    width: height / 2
    height: parent.height
    scale: PathView.iconScale

    // Resets the CoverFlow delegate (and it's children i.e. ImageView) to the
    // original state. That is, showing just the thumbnail on the path.
    function reset() {
        console.log("Hiding the largeImage and returning to orig state");
        if (largeImage.state == "visible" || largeImage.state == "info") {
            console.log("LargeImage visible! Closing...");
            largeImage.close();
        }
    }

    // The coverflow item consists of two images with white borders.
    // The second image is flipped and has some opacity for nice mirror effect.
    Column  {
        id: delegate
        y: coverFlow.topMargin
        spacing: 5

        // White borders around the image
        Rectangle {
            id: delegateImage

            // The rectangle is a square.
            width: delegateItem.width
            height: delegateImage.width
            color: dlgImg.status == Image.Ready ? "white" : "transparent"

            // Should go on top of the reflection image when zooming.
            z: reflection.z + 1

            Image {
                id: dlgImg

                width: delegateImage.width - 8
                height: delegateImage.height - 8
                anchors.centerIn: parent

                // Only set sourceSize.width or height to maintain aspect ratio.
                sourceSize.width: delegateImage.width
                sourceSize.height: delegateImage.height
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
            height: delegateImage.height

            Image {
                id: reflection

                width: delegateImage.width
                height: delegateImage.height
                anchors.centerIn: parent
                sourceSize.width: delegateImage.width
                sourceSize.height: delegateImage.height
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

                // NOTE: This does not work when there's 3D transformations
                // (like rotation around Y-axis or X-axis)
                //AlphaGradient {}
            }
        }
    }

    // View showing the real image instead of the upscaled version of it.
    ImageView {
        id: largeImage
        anchors.centerIn: parent
        // Define the maximum size for the ImageView. The more accurate
        // size will be depending on the size of the image itself (the
        // aspect ratio of the image will be preserved).
        maxWidth: coverFlow.width - 80
        maxHeight: coverFlow.height - 20
        z: parent.z + 1

        fillMode: Image.PreserveAspectFit

        onClosed: {
            console.log("Closing largeImage")
            delegateItem.state = ""
        }
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if (pathView.currentIndex == index) {
                console.log("Clicked on current item, zoom it")

                parent.state == "scaled" ? parent.state = "" : parent.state = "scaled"

                // Switch to the ImageView
                largeImage.imagePath = url
                largeImage.state = "visible"
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
                // Scale up the icon
                scale: 1.8
            }
            PropertyChanges {
                target: delegateImage
                opacity: 0.001 // Hide the icon practically completely
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "scaled"
            reversible: true
            ParallelAnimation {
                PropertyAnimation {
                    target: delegateImage
                    properties: "scale"
                    duration: 300
                }
                PropertyAnimation {
                    target: delegateImage
                    properties: "opacity"
                    duration: 300
                }
            }
        }
    ]
}
