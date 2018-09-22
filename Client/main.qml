import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.2

ApplicationWindow {
    visible: true
    visibility:Window.FullScreen
    width: 960
    height: 540
    ControlPanel{
        id:controlPanel
    }
    Setting{
        id:setting
    }
    BackGround{
        id:backGround
        anchors.fill: parent
    }
    StackView{
        id:stackView
        anchors.fill: parent
        initialItem: controlPanel

        focus: true
        Keys.enabled: true
        Keys.onEscapePressed: Qt.quit()

        pushEnter: Transition {
            id: pushEnter
            NumberAnimation{
                properties: "y"
                from: -stackView.height
                to:0
                duration: 324
                easing.type: Easing.InExpo
            }
        }
        pushExit: Transition{
            id:pushExit
            NumberAnimation{
                properties: "y"
                to:stackView.height
                duration: 324
                easing.type: Easing.InExpo
            }
        }
        popEnter: Transition {
            id:popEnter
            NumberAnimation{
                properties: "y"
                to:0
                duration: 324
                easing.type: Easing.OutExpo
            }
        }
        popExit: Transition {
            id: popExit
            NumberAnimation{
                properties: "y"
                to:-stackView.height
                duration: 324
                easing.type: Easing.OutExpo
            }
        }
    }
    Connections{
        target: controlPanel
        onPageChanged:{
            stackView.push(setting)
        }
        onMessageChecked:{
            network.messageChanged(flag)
        }
        onCameraChecked:{
            network.cameraChanged(flag)
        }
        onRadarChecked:{
            network.radarChanged(flag)
        }
        onDirectionChanged: {
            network.directionChanged(controlPanel.direction)
        }
    }
    Connections{
        target: setting
        onPageChanged:{
            stackView.pop()
        }
        onConnectSwitchChecked:{
            if(flag){
                network.connectServer()
            }
            else{
                network.disconnectServer()
            }
        }
        onLockSwitchChecked:{
            controlPanel.notLock=!flag
        }
    }
    Connections{
        target: network
        onConnectStateChanged:{
            setting.connectStateChanged(flag)
        }
        onCameraStateChanged:{
            controlPanel.cameraStateChanged(flag)
        }
        onMessageStateChanged:{
            controlPanel.messageStateChanged(flag)
        }
        onRadarStateChanged:{
            controlPanel.radarStateChanged(flag)
        }
        onCallQmlRefeshImage:{
            //backGround.source=""
            backGround.source="image://codeimg/"+Math.random()
        }
    }
}
