import QtQuick 1.0

Rectangle {
    width: 340
    height: 340
    color: "gray"

    property string infoText: "image.jpg"
    signal closed()

    Text {
        width: parent.width
        height: parent.height

        wrapMode: Text.Wrap
        text: infoText
    }

    Button {
        anchors {
            bottom: parent.bottom
            right: parent.right
            margins: 30
        }

        buttonName: "infoButton"
        bgImage: "gfx/exit_button.png"
        bgImagePressed: "gfx/exit_button_pressed.png"

        width: 66
        height: 66
        opacity: 0.6

        onClicked: {
            console.log("infoView closebutton clicked")
            parent.closed()
        }
    }

    // Mirroring rotation applied beforehand, because the ImageView will
    // rotate itself 180 degrees to show the "background" (which is InfoView)
    transform: Rotation {
        id: flip
        origin.x: parent.width / 2
        origin.y: parent.height / 2
        axis { x: 0; y: 1; z: 0 } // Rotate around y-axis.
        angle: 180
    }
}
