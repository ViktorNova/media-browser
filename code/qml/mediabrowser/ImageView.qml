import QtQuick 1.0

Item {
    id: imageView

    property string imagePath: ""
    property int fillMode: Image.PreserveAspectFit
    signal closed();

    visible: false

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            imageView.closed();
            console.log("imageView's mouseArea clicked")
        }
    }

    Image {
        id: img
        fillMode: Image.PreserveAspectFit
        width: imageView.width
        height: imageView.height

        sourceSize.width: imageView.width
        sourceSize.height: imageView.height

        // Setting this will provide a larger image
        source: "image://imageprovider/full/" + imagePath
    }
}
