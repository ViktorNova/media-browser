/**
 * Copyright (c) 2012-2014 Microsoft Mobile.
 */

import QtQuick 1.1
import com.nokia.meego 1.0

// Includes for the Video Player.
import "VideoPlayer"                // Video Player module
import QtMobility.systeminfo 1.1    // voiceRingtoneVolume

Window {
    id: mainView

    Component.onCompleted: {
        // Use the black theme on MeeGo.
        theme.inverted = true;
        // Force the screen in landscape orientation.
        screen.allowedOrientations = Screen.Landscape;
        // Also, set the Window's width correctly according the largest
        // dimension (coz it's different in E6, which screen is always in LS).
        width = screen.displayHeight > screen.displayWidth
                ? screen.displayHeight : screen.displayWidth

        if (galleryModel.isReady) {
            busyIndicatorLoader.sourceComponent = undefined;
        }

        // This is to work around issues related to forcing the application to
        // landscape in MeeGo. For some reason, if the CoverFlow isn't created
        // after when the screen.allowedOrientations = Screen.Landscape has
        // taken into effect, some of the CoverFlowDelegates might be created
        // beforehand and thus when finally in landscape, they are shown
        // incorrectly...
        coverFlowLoader.sourceComponent = coverFlow;
    }

    // VisualStyle needed for VideoPlayer Component.
    VisualStyle {
        id: visual
        // Set the initial volume level (from the device's profile) for videos.
        DeviceInfo {id: devInfo}
        currentVolume: devInfo.voiceRingtoneVolume / 100
    }
    
    // Background image
    Image {
        id: background

        width: parent.width
        height: parent.height
        source: "gfx/background_n8.png"
    }

    // NOTE: CoverFlow inside a loader because weird screen orientation issues
    // in MeeGo. Read more from above ^.
    Loader {
        id: coverFlowLoader
        anchors.fill: parent

        Component {
            id: coverFlow

            CoverFlow {
                anchors.fill: parent
            }
        }
    }

    Loader {
        id: busyIndicatorLoader

        width: 150
        height: 150
        anchors.centerIn: parent

        sourceComponent: busyIndicator
        Component {
            id: busyIndicator

            BusyIndicator {
                running: true
            }
        }
    }

    Connections {
        target: galleryModel
        onBrowserModelReady: {
            busyIndicatorLoader.item.running = false;
            busyIndicatorLoader.sourceComponent = undefined;
        }
    }
}
