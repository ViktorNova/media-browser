import QtQuick 1.0

Item {
    id: imageView

    property string imagePath: ""
    property int fillMode: Image.PreserveAspectFit
    signal closed();

    opacity: 0

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

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        onClicked: {
            imageView.closed();
            console.log("imageView's mouseArea clicked")

            parent.state == "visible" ?
                parent.state = "" : parent.state = "visible"
        }
    }

    states: [
        State {
            name: "visible"
            PropertyChanges {
                target: largeImage
                opacity: 1.0
            }
        }
    ]

    transitions: [
        Transition {
            from: ""
            to: "visible"
            reversible: true

            PropertyAnimation {
                target: largeImage
                properties: "opacity"
                duration: 500
            }
        }
    ]
}
