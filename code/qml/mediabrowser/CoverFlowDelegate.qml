import QtQuick 1.0

Rectangle {
    id: delegateItem
    y: container.topMargin
    x: 0
    z: PathView.z
    width: delegateItem.height
    height: container.height - container.topMargin - container.bottomMargin
    scale: PathView.iconScale
    color: "blue"
    radius: 10
    Image {
        clip: true
        id: delegateImage
        anchors.centerIn: parent
        width: delegateImage.height
        height: parent.height - 20
        source: url
        fillMode: Image.PreserveAspectCrop
        asynchronous: true
        // Only set sourceSize.width or height to maintain aspect ratio
        sourceSize.width: delegateImage.width
    }
    transform: Rotation {
        origin.x: delegateImage.width/2; origin.y: delegateImage.height/2
        axis.x: 0; axis.y: 1; axis.z: 0     // rotate around y-axis
        angle: PathView.angle
    }
}
