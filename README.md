Media Browser v2.0
==================

The Media Browser example application demonstrates how to use the Document
Gallery (part of QtMobility APIs), create a visually appealing coverflow view,
and show image and video files. It also demonstrates the use of QML image
provider plugin written in native Qt C++. The UI is implemented using mostly
custom graphics and UI elements, but the updated version uses also some
Qt Quick Components.

The updated application uses a customised QML Video player which has been made
so that it can easily be used in any Qt Quick application. The video player
component is developed in the Video Streamer project, available at:
https://github.com/nokia-developer/video-streamer.


This example application demonstrates:
- how to create a smooth, graphics intensive, custom GUI with QML
- creating a coverflow-like component with QML PathView
- QML video playing capabilities
- using the custom QML video player component
- using Qt Quick Components in an application that supports multiple
  resolutions and both touch UI and keypad
- handling volume keys of a Symbian phone within the QML code

This example application is hosted in GitHub:
https://github.com/nokia-developer/media-browser

For more information on the implementation, visit the wiki page:
https://github.com/nokia-developer/media-browser/wiki


1. Usage
-------------------------------------------------------------------------------

The QML Media Browser application has an intuitive user interface, which
requires merely swiping to move along the cover flow path, and tapping the
images to enlarge and shrink them.

The customised QML VideoPlayer component can be taken into use in 3rd party
applications simply by adding the VideoPlayer QML sources into the project and
importing it into a QML file. The player itself can be created simply by
instantiating the VideoPlayView and by calling its setVideoData() function.

Also, the VisualStyle should be instantiated somewhere within the application
in order for the video controls to show correctly. The VisualStyle can also be
used for tracking and setting the current volume for the video files.


2. Prerequisites
-------------------------------------------------------------------------------

 - Qt basics
 - Qt Quick basics
 - Qt Quick Components basics


3. Project structure and implementation
-------------------------------------------------------------------------------

3.1 Folders
-----------

 |                   The root folder contains the licence information and
 |                   this file (release notes).
 |
 |- design           Contains UX design files.
 |
 |- screenshots      Contains screenshots taken from the application.
 |
 |- meego            MeeGo 1.2 Harmattan platform-specific code files.
 |  |
 |  |- code          Root folder for project, gfx, QML, and Javascript files.
 |  |
 |  |- qtc_packaging Contains the MeeGo 1.2 Harmattan (Debian) packaging files.
 |
 |- symbian          Symbian (Anna/Belle) platform-specific code files.
 |  |
 |  |- code          Root folder for project, gfx, QML, and Javascript files.
 |


3.2 Important files and classes
-------------------------------

| Class                   | Description                                       |
|-------------------------|---------------------------------------------------|
| ImageScaler             | A class that walks through all image files once   |
|                         | on startup and creates the thumbnail images.      |
|-------------------------|---------------------------------------------------|
| ImageProvider           | A class that provides the thumbnails, reflection  |
|                         | images and screen images to QML GUI.              |
|-------------------------|---------------------------------------------------|
| LoadHelper              | Helper class that is used to load the main QML    |
|                         | file right after showing the splash screen.       |
|-------------------------|---------------------------------------------------|
| VolumeKeys              | Class that implements the MRemConCoreApiTarget-   |
|                         | Observer for reacting to hardware volume buttons. |
|-------------------------|---------------------------------------------------|


3.3 Used APIs/QML elements/Qt Quick Components
----------------------------------------------

The following APIs, QML elements, and Qt Quick Components have been used. 

Qt:
- QAbstractListModel
- QDir
- QFile
- QImage
- QImageReader
- QPainter
- QThread
- QTimer
- QUrl

Qt Mobility:
- QDocumentGallery
- QGalleryQueryRequest
- QGalleryResultSet

Standard QML elements:
- Connections
- Path
- PathAttribute
- PathView
- Loader
- Text
- Timer

- Behavior
- ParallelAnimation
- Rotation
- SequentialAnimation
- State
- Transition

QML elements from Qt Quick Components:
- BusyIndicator
- Button
- ProgressBar
- Slider
- ToolButton

QML elements from Qt Mobility:
- DeviceInfo
- Video


4. Compatibility
-------------------------------------------------------------------------------

Compatible with:
 - Symbian devices with Qt 4.7.4 or higher.
 - MeeGo 1.2 Harmattan devices.

Tested on:
 - Nokia E6
 - Nokia E7-00
 - Nokia N9

Developed with:
 - Qt SDK 1.2


4.1 Required capabilities
-------------------------

None; The application can be self signed on Symbian.


4.2 Known issues and design limitations
---------------------------------------

After video playback there are performance bugs.
- https://bugreports.qt.nokia.com/browse/QTMOBILITY-1570 & -1818

Swiping the application to the background on the Nokia N9 while the video is
playing may cause the application to exit.
- https://bugreports.qt-project.org/browse/QTMOBILITY-1995


5. Building, installing, and running the application
-------------------------------------------------------------------------------

5.1 Preparations
----------------

Check that you have the latest Qt SDK installed in the development environment
and the latest Qt version on the device.

Qt Quick Components 1.1 or higher is required.

5.2 Using the Qt SDK
--------------------

You can install and run the application on the device by using the Qt SDK.
Open the project in the SDK, set up the correct target (depending on the device
platform), and click the Run button. For more details about this approach,
visit the Qt Getting Started section at Nokia Developer
(http://www.developer.nokia.com/Develop/Qt/Getting_started/).

5.3 Symbian device
------------------

Make sure your device is connected to your computer. Locate the .sis
installation file and open it with Nokia Suite. Accept all requests from Nokia
Suite and the device. Note that you can also install the application by copying
the installation file onto your device and opening it with the Symbian File
Manager application.

After the application is installed, locate the application icon from the
application menu and launch the application by tapping the icon.

5.4 Nokia N9
------------

Copy the application Debian package onto the device. Locate the file with the
device and run it; this will install the application. Note that you can also
use the terminal application and install the application by typing the command
'dpkg -i <package name>.deb' on the command line. To install the application
using the terminal application, make sure you have the right privileges 
to do so (e.g. root access).

Once the application is installed, locate the application icon from the
application menu and launch the application by tapping the icon.


6. Licence
-------------------------------------------------------------------------------

See the licence text file delivered with this project. The licence file is also
available online at
https://github.com/nokia-developer/media-browser/blob/master/Licence.txt


7. Related documentation
-------------------------------------------------------------------------------
Qt Quick Components
- http://doc.qt.nokia.com/qt-components-symbian/index.html
- http://harmattan-dev.nokia.com/docs/library/html/qt-components/qt-components.html


8. Version history
-------------------------------------------------------------------------------
2.0 New enhanced version with video playback support, fine-tuned UI,
    and improved performance.
1.0 Published on the Nokia Developer website.

