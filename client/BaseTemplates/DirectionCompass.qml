import QtQuick 2.11
import "qrc:/algorithm.js" as MyMath

Item {
    id:directionCompass

    property bool checked:false
    property double size:0
    property double coordX:0
    property double coordY:0
    property double direction:-1/*(0~2PI)
                                  -1    静止
                                  左上    第三象限
                                  左下    第二象限
                                  右下    第一象限
                                  右下    第四象限
                                 */
    property double distance:0

    Rectangle{
        id:bigRoulette
        color: "white"
        opacity: 0.3
    }

    Rectangle{
        id:smallRoulette
        color: "white"
        opacity: 0.3
    }

    MultiPointTouchArea{
        id:touchArea
        anchors.fill: directionCompass
        touchPoints: [
            TouchPoint {
                id: mouseArea
            }
        ]

        onPressed: {
            if(MyMath.distance(0.5*size,0.5*size,mouseArea.x,mouseArea.y)<0.5*size)
                checked=true
        }

        onReleased: {
            checked=false
        }
        onUpdated: {
            distance=MyMath.distance(0.5*size,0.5*size,mouseArea.x,mouseArea.y)
            if(distance>0.5*size)distance=0.5*size
            direction=MyMath.angle(0.5*size,0.5*size,mouseArea.x,mouseArea.y)
            console.log(direction)
        }
    }

    onSizeChanged: {
        width=size
        height=size
        x=coordX-0.5*width
        y=coordY-0.5*height

        bigRoulette.width=size
        bigRoulette.height=size
        bigRoulette.x=0.5*(width-bigRoulette.width)
        bigRoulette.y=0.5*(height-bigRoulette.height)
        bigRoulette.radius=size/2

        smallRoulette.width=0.45*size
        smallRoulette.height=0.45*size
        smallRoulette.x=0.5*(width-smallRoulette.width)
        smallRoulette.y=0.5*(height-smallRoulette.height)
        smallRoulette.radius=0.45*size/2
    }

    onCoordXChanged: {
        x=coordX-0.5*width
        bigRoulette.x=0.5*(width-bigRoulette.width)
        smallRoulette.x=0.5*(width-smallRoulette.width)
    }

    onCoordYChanged: {
        y=coordY-0.5*height
        bigRoulette.y=0.5*(height-bigRoulette.height)
        smallRoulette.y=0.5*(height-smallRoulette.height)
    }

    onCheckedChanged: {
        if(!checked){
            direction=-1
            smallRoulette.x=0.5*(width-smallRoulette.width)
            smallRoulette.y=0.5*(height-smallRoulette.height)
        }
    }

    onDirectionChanged: {
        if(checked){
            var distanceX=0.5*size+distance*Math.cos(direction)
            var distanceY=0.5*size+distance*Math.sin(direction)
            smallRoulette.x=distanceX-0.5*smallRoulette.width
            smallRoulette.y=distanceY-0.5*smallRoulette.height
        }
    }
}
