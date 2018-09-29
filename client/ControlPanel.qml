import QtQuick 2.11
import "qrc:/BaseTemplates/" as MyCode

Item {
    id:controlPanel
    visible:false

    property alias connectButton:connect
    property alias cameraButton:camera
    property alias radarButton:radar

    signal pageChanged()
    signal connectStateChanged(var flag)
    signal cameraStateChanged(var flag)
    signal radarStateChanged(var flag)

    MyCode.DirectionCompass{
        id:directionCompass
        coordX:controlPanel.height/4
        coordY:controlPanel.height-controlPanel.height/4
        size:controlPanel.height/4
    }

    MyCode.RoundButton{
        id:connect
        coordX:controlPanel.width-Math.cos(Math.PI/8)*0.4*controlPanel.height
        coordY:controlPanel.height-Math.sin(Math.PI/8)*0.4*controlPanel.height
        size: controlPanel.height/8
        source:"qrc:/Images/message.png"
        onClicked: {
            connectStateChanged(!checked)
        }
    }

    MyCode.RoundButton{
        id:camera
        coordX:controlPanel.width-Math.cos(Math.PI/8*2)*0.4*controlPanel.height
        coordY:controlPanel.height-Math.sin(Math.PI/8*2)*0.4*controlPanel.height
        size: controlPanel.height/8
        source:"qrc:/Images/camera.png"
        onClicked: {
            cameraStateChanged(!checked)
        }
    }

    MyCode.RoundButton{
        id:radar
        coordX:controlPanel.width-Math.cos(Math.PI/8*3)*0.4*controlPanel.height
        coordY:controlPanel.height-Math.sin(Math.PI/8*3)*0.4*controlPanel.height
        size: controlPanel.height/8
        source:"qrc:/Images/radar.png"
        onClicked: {
            radarStateChanged(!checked)
        }
    }

    MyCode.RoundButton{
        id:quit
        coordX: parent.width-0.75*size
        coordY: size
        size: controlPanel.height/16
        source:"qrc:/Images/shutdown.png"
        onClicked: {
            Qt.quit()
        }
        onPressed: {
            timer.start()
        }
        onReleased: {
            timer.stop()
        }

        Timer {
            id:timer
            interval: 1000
            running: false
            repeat: false
            onTriggered: {
                setting.visible=!setting.visible
            }
        }
    }

    MyCode.RoundButton{
        id:setting
        visible: false
        coordX: parent.width-2*size
        coordY: size
        size: controlPanel.height/16
        source:"qrc:/Images/setting.png"
        onClicked: {
            pageChanged()
        }
    }
}
