import QtQuick 2.11
import QtGraphicalEffects 1.0

Item {
    opacity: 0.4
    property alias imageSource:image.source
    signal clickedChanged()
    Rectangle{
        id:ground
        anchors.fill: parent
        radius: width/2
        visible: false
    }
    Image {
        id: image
        visible: false
    }
    OpacityMask{
        id:button
        visible: true
        anchors.fill:ground
        source: image
        maskSource: ground
    }
    MouseArea{
        id:area
        anchors.fill:button
        onClicked: {
            clickedChanged()
        }
    }
    function opacityChanged(flag){
        if(flag)opacity=0.8
        else opacity=0.4
    }
}
