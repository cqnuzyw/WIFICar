import QtQuick 2.11
import QtQuick.Controls 2.3

Item{
    signal pageChanged()//子页面发出切换信号
    signal connectSwitchChecked(var flag)//连接按钮被点击时触发
    signal lockSwitchChecked(var flag)//锁定按钮被点击时触发
    Rectangle{
        id:board
        width: 0.9*parent.width
        height: 0.9*parent.height
        anchors.centerIn: parent
        color: "#cccccc"
        opacity: 0.6
    }
    Label{
        id: connectLabel
        width: 0.3*board.width
        height: 0.1*board.height
        anchors.left: board.left
        anchors.leftMargin: 0.1*board.width
        anchors.top: board.top
        anchors.topMargin: 0.1*board.height
        text: qsTr("连接")
        font.pixelSize: 0.8*height
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }

    Switch {
        id: connectSwitch
        width: 0.3*board.width
        height: 0.1*board.height
        anchors.right: board.right
        anchors.rightMargin: 0.05*board.width
        anchors.top: board.top
        anchors.topMargin: 0.1*board.height
        font.pixelSize: 0.8*height
        text:qsTr("未连接")
        onCheckedChanged: {
            connectSwitchChecked(checked)
        }
    }

    Label{
        id: lockLabel
        width: 0.3*board.width
        height: 0.1*board.height
        anchors.left: board.left
        anchors.leftMargin: 0.1*board.width
        anchors.top: connectLabel.bottom
        anchors.topMargin: 0.1*board.height
        text: qsTr("轮盘")
        font.pixelSize: 0.8*height
        verticalAlignment: Text.AlignVCenter
        horizontalAlignment: Text.AlignHCenter
    }
    Switch {
        id: lockSwitch
        width: 0.3*board.width
        height: 0.1*board.height
        anchors.right: board.right
        anchors.rightMargin: 0.05*board.width
        anchors.top: connectSwitch.bottom
        anchors.topMargin: 0.1*board.height
        font.pixelSize: 0.8*height
        text:qsTr("固定")
        onCheckedChanged: {
            lockSwitchChecked(checked)
            if(checked)text="移动"
            else text="固定"
        }
    }
    Rectangle{
        id: returnButton
        width: 0.16*board.width
        height: 0.1*board.height
        anchors.horizontalCenter: board.horizontalCenter
        anchors.bottom: board.bottom
        anchors.bottomMargin: 0.1*board.height
        radius: 0.1*height
        color: "black"
        Text{
            anchors.fill: parent
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            text: qsTr("返   回")
            color: "white"
            font.pixelSize: 0.5*height
        }
        MouseArea{
            anchors.fill: parent
            onClicked: {
                pageChanged()
            }
        }
    }
    function connectStateChanged(flag){//当c++端socket状态变化时执行
        switch(flag){
        case 0:
            connectSwitch.text="未连接";
            connectSwitch.checked=false;
            break;
        case 1:
            connectSwitch.text="正在执行主机名查找";break;
        case 2:
            connectSwitch.text="已开始建立连接";break;
        case 3:
            connectSwitch.text="建立连接";
            connectSwitch.checked=true;
            break;
        case 4:
            connectSwitch.text="绑定到地址和端口";break;
        case 5:
            connectSwitch.text="即将关闭";
            connectSwitch.checked=false;
            break;
        case 6:
            connectSwitch.text="仅限内部使用";break;
        }
    }
}
