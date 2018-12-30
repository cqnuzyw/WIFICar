import QtQuick 2.11
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import "qrc:/BaseTemplates/" as MyCode
import "qrc:/algorithm.js" as MyMath

ApplicationWindow{
    width: 960
    height: 540
    visible: true
    visibility:Window.FullScreen

    ControlPanel{
        id:controlPanel
    }

    SettingPanel{
        id:settingPanel
    }

    background:Image {
        id:backGroundImage
        anchors.fill: parent
        source: "qrc:/Images/bg.png"
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
        target: settingPanel
        onPageChanged:{
            stackView.pop()
        }
    }

    Connections{
        target: controlPanel
        onPageChanged:{
            stackView.push(settingPanel)
        }
        onConnectStateChanged:{
            if(flag){
                client.connectToHost("192.168.43.93");
                client.sendControlMessage(MyMath.Connect,flag)
            }
            else{
                client.disconnectFromHost();
                controlPanel.connectButton.checked=false;
                client.cameraStateChanged(false)
            }

        }
        onCameraStateChanged:{
            client.sendControlMessage(MyMath.Camera,flag)
        }
        onRadarStateChanged:{
            client.sendControlMessage(MyMath.Radar,flag)
        }
    }
    Connections{
        target: client
        onConnectStateChanged:{
            controlPanel.connectButton.checked=flag
        }
        onCameraStateChanged:{
            controlPanel.cameraButton.checked=flag
            if(!flag){
                backGroundImage.source="qrc:/Images/bg.png"
            }
        }
        onRadarStateChanged:{
            controlPanel.radarButton.checked=flag
        }
        onCallQmlRefeshImage:{
            if(controlPanel.cameraButton.checked){
                backGroundImage.source="image://background/"+Math.random()
            }
        }
    }
}
