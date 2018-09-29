import QtQuick 2.11
import QtGraphicalEffects 1.0
import "qrc:/algorithm.js" as MyMath

Item{
    id:roundButton
    opacity:0.4

    property bool checked:false
    property double size:0
    property double coordX:0
    property double coordY:0
    property alias source:image.source
    signal clicked()
    signal pressed()
    signal released()

    Rectangle{
        id:background
        anchors.fill: roundButton
        radius: width/2
        visible: false
    }

    Image{
        id:image
        visible: false
    }

    OpacityMask{
        id:button
        visible: true
        anchors.fill: background
        source: image
        maskSource: background
    }

    MultiPointTouchArea{
        id:touchArea
        anchors.fill: button
        touchPoints: [
            TouchPoint {
                id: mouseArea
            }
        ]
        onPressed: {
            roundButton.pressed()
        }

        onReleased: {
            roundButton.released()
            if(MyMath.distance(0.5*width,0.5*height,mouseArea.x,mouseArea.y)<0.5*size){
                clicked()
            }
        }
    }

    onCheckedChanged: {
        if(checked)opacity=0.8
        else opacity=0.4
    }

    onSizeChanged: {
        width=size
        height=size
        x=coordX-0.5*width
        y=coordY-0.5*height
    }

    onCoordXChanged: {
        x=coordX-0.5*width
    }

    onCoordYChanged: {
        y=coordY-0.5*height
    }
}
