import QtQuick 2.11

Item {
    property alias source:bgImage.source
    Image{
        id: bgImage
        anchors.fill: parent
        source:"qrc:/Images/bg.png"
    }
}
