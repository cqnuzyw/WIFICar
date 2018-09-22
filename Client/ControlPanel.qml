import QtQuick 2.11
import "Qml"
import "Algorithm.js" as MyMath

Item {
    signal pageChanged()
    property double size: width<height?width:height
    property double circle_size: Math.PI*size/12
    property bool notMove: true
    property bool notLock: true
    property var direction

    signal messageChecked(var flag)
    signal radarChecked(var flag)
    signal cameraChecked(var flag)
    signal lockChanged(var flag)

    CircleButton{
        id:outside_circle
        coordX: 0.25*size
        coordY: parent.height-0.25*size
        width: 0.3*size
        height: 0.3*size
        opacity: 0.1
    }
    CircleButton{
        id:inside_circle
        coordX: 0.25*size
        coordY: parent.height-0.25*size
        width: 0.4*outside_circle.width
        height: 0.4*outside_circle.height
        opacity: 0.3
    }
    MouseArea{
        width: 0.5*parent.width
        height: parent.height
        anchors.left: parent.left
        anchors.verticalCenter: parent.verticalCenter
        onPressed: {
            if(!notLock){
                var coordX=mouseX
                var coordY=mouseY
                if(coordX<0.5*outside_circle.width)coordX=0.5*outside_circle.width
                if(coordX>width-0.5*outside_circle.width)coordX=width-0.5*outside_circle.width
                if(coordY<0.5*outside_circle.height)coordY=0.5*outside_circle.height
                if(coordY>height-0.5*outside_circle.height)coordY=height-0.5*outside_circle.height
                outside_circle.coordX=coordX
                outside_circle.coordY=coordY

                coordX=mouseX
                coordY=mouseY
                if(coordX<0.5*inside_circle.width)coordX=0.5*inside_circle.width
                if(coordX>width-0.5*inside_circle.width)coordX=width-0.5*inside_circle.width
                if(coordY<0.5*inside_circle.height)coordY=0.5*inside_circle.height
                if(coordY>height-0.5*inside_circle.height)coordY=height-0.5*inside_circle.height
                inside_circle.coordX=coordX
                inside_circle.coordY=coordY
            }
            if(MyMath.distance(mouseX,mouseY,outside_circle.coordX,outside_circle.coordY)<=0.5*outside_circle.width){
                notMove=false
            }
        }
        onReleased: {
            notMove=true
        }

        onPositionChanged: {
            if(!notMove){
                var coordX=mouseX
                var coordY=mouseY
                if(coordX<0.5*inside_circle.width)coordX=0.5*inside_circle.width
                if(coordX>width-0.5*inside_circle.width)coordX=width-0.5*inside_circle.width
                if(coordY<0.5*inside_circle.height)coordY=0.5*inside_circle.height
                if(coordY>height-0.5*inside_circle.height)coordY=height-0.5*inside_circle.height
                var distance=MyMath.distance(coordX,coordY,outside_circle.coordX,outside_circle.coordY)
                if(distance>0.5*outside_circle.width){
                    distance=0.5*outside_circle.width
                    direction=Math.atan((outside_circle.coordY-coordY)/(coordX-outside_circle.coordX))
                    if(coordX<outside_circle.coordX)direction-=Math.PI
                    coordX=outside_circle.coordX+distance*Math.cos(direction)
                    coordY=outside_circle.coordY-distance*Math.sin(direction)
                }
                inside_circle.coordX=coordX
                inside_circle.coordY=coordY
            }
        }
    }
    onNotMoveChanged: {
        if(notMove){
            inside_circle.coordX=outside_circle.coordX
            inside_circle.coordY=outside_circle.coordY
        }
    }
    onNotLockChanged: {
        if(notLock){
            outside_circle.coordX=0.25*size
            outside_circle.coordY=parent.height-0.25*size
            inside_circle.coordX=outside_circle.coordX
            inside_circle.coordY=outside_circle.coordY
        }
    }

    RoundButton{
        id:message
        x:parent.width-Math.cos(Math.PI/10)*0.5*size
        y:parent.height-Math.sin(Math.PI/10)*0.5*size
        width: 0.5*circle_size
        height: 0.5*circle_size
        imageSource: "qrc:/Images/message.png"
        onClickedChanged:{
            messageChecked(message.opacity<0.5)
        }
    }

    RoundButton{
        id:camera
        x:parent.width-Math.cos(Math.PI/10*2)*0.5*size
        y:parent.height-Math.sin(Math.PI/10*2)*0.5*size
        width: 0.5*circle_size
        height: 0.5*circle_size
        imageSource:"qrc:/Images/camera.png"
        onClickedChanged: {
            cameraChecked(camera.opacity<0.5)
        }
    }
    RoundButton{
        id:radar
        x:parent.width-Math.cos(Math.PI/10*3)*0.5*size
        y:parent.height-Math.sin(Math.PI/10*3)*0.5*size
        width: 0.5*circle_size
        height: 0.5*circle_size
        imageSource:"qrc:Images/radar.png"
        onClickedChanged: {
            radarChecked(radar.opacity<0.5)
        }
    }
    RoundButton{
        id:lock
        x:parent.width-Math.cos(Math.PI/10*4)*0.5*size
        y:parent.height-Math.sin(Math.PI/10*4)*0.5*size
        width: 0.5*circle_size
        height: 0.5*circle_size
        imageSource:"qrc:/Images/lock.png"
        onClickedChanged: {
            lockChanged(lock.opacity<0.5)
            opacityChanged(lock.opacity<0.5)
        }
    }

    RoundButton{
        id:setting
        anchors.right: shutdown.left
        anchors.rightMargin: 0.05*circle_size
        anchors.top: shutdown.top
        width: 0.25*circle_size
        height: 0.25*circle_size
        imageSource: "qrc:/Images/setting.png"
        onClickedChanged: {
            pageChanged()
        }
    }

    RoundButton{
        id:shutdown
        x:parent.width-width-0.05*circle_size
        y:0.05*circle_size
        width: 0.25*circle_size
        height: 0.25*circle_size
        imageSource: "qrc:/Images/shutdown.png"
        onClickedChanged: {
            Qt.quit()
        }
    }
    function cameraStateChanged(flag){
        camera.opacityChanged(flag)
    }
    function messageStateChanged(flag){
        message.opacityChanged(flag)
    }
    function radarStateChanged(flag){
        radar.opacityChanged(flag)
    }
    onLockChanged: {
        notLock=flag
    }
}
