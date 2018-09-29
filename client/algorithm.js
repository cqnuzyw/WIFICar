var Connect=0x0F00
var Camera=0x0F01
var Radar=0x0F02
var Message=0x0F03
var PhotoSend=0x0F04
var DirectSend=0x0F05

function angle(x1,y1,x2,y2){
    var distanceX=x2-x1
    var distanceY=y2-y1
    if(distanceX<0)
        return Math.atan(distanceY/distanceX)+Math.PI
    else if(distanceY<0)
        return Math.atan(distanceY/distanceX)+Math.PI*2
    return Math.atan(distanceY/distanceX)
}

function distance(x1,y1,x2,y2){
    return Math.pow(Math.pow(x1-x2,2)+Math.pow(y1-y2,2),0.5)
}
