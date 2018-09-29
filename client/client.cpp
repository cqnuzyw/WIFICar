#include "client.h"
#include <iostream>

Client::Client(QObject *parent)
    :QObject{parent},_imageSize{0}
{
    _control=new TcpSocket(this);
    _camera=new TcpSocket(this);
    _radar=new TcpSocket(this);
    m_imageProvider=new ImageProvider;

    connect(this->_control,&TcpSocket::readyRead,this,&Client::receiveControlMessage);
    connect(this->_camera,&TcpSocket::readyRead,this,&Client::receiveCameraMessage);
}

Client::~Client()
{
    disconnectFromHost();
    //delete m_ImageProvider;
    delete _control;
    delete _camera;
    delete _radar;
}

bool Client::connectToHost(const QString &address)
{
    if(!_control->connectToServer(address,ControlPORT)){
        return false;
    };
    if(!_camera->connectToServer(address,CameraPORT)){
        return false;
    };
    /*if(!_radar->connectToServer(address,RadarPORT)){
        return false;
    };*/
    return true;
}

bool Client::disconnectFromHost()
{
    if(!_control->disconnectFromServer()){
        return false;
    };
    if(!_camera->disconnectFromServer()){
        return false;
    };
    /*if(!_radar->disconnectFromServer()){
        return false;
    }*/
    return true;
}

bool Client::sendControlMessage(qint64 sign, qint64 flag) const
{
    QByteArray block;
    QDataStream output(&block,QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_11);
    output<<qint64(sign)<<qint64(flag);

    qint64 len=_control->sendMessage(block);

    block.resize(0);
    if(len<0)return false;
    return len;
}

bool Client::receiveControlMessage()
{
    QByteArray block;
    block=_control->receiveMessage(sizeof(qint64)*2);
    QDataStream output(&block,QIODevice::ReadOnly);
    output.setVersion(QDataStream::Qt_5_11);

    qint64 sign,flag;
    output>>sign>>flag;
    block.resize(0);
    switch(sign){
    case Connect:emit connectStateChanged(flag);break;
    case Camera:emit cameraStateChanged(flag);break;
    case Radar:emit radarStateChanged(flag);break;
    case Message:emit messageStateChanged(flag);break;
    default:break;
    }
    return true;
}

bool Client::receiveCameraMessage()
{
    if(_imageSize==0){
        _startTime=clock();
        QByteArray num=_camera->receiveMessage(sizeof(quint64));
        QDataStream output(&num,QIODevice::ReadOnly);
        output.setVersion(QDataStream::Qt_5_11);
        output>>_imageSize;
    }
    if(_imageSize>0){
        QByteArray message=_camera->receiveMessage(_imageSize);
        if(!message.size()){
            return false;
        }
        QDataStream in(&message,QIODevice::ReadOnly);
        QByteArray date;
        in.setVersion(QDataStream::Qt_5_11);
        in>>date;
        QString ss=QString::fromLatin1(date.data(),date.size());
        QByteArray rc;
        rc=QByteArray::fromBase64(ss.toLatin1());
        QByteArray rdc=qUncompress(rc);
        bool flag=m_imageProvider->image.loadFromData(rdc,"JPEG");
        if(flag){
            emit callQmlRefeshImage();
            _endTime=clock();
        }
        _imageSize=0;
    }
    return true;
}

bool Client::receiveRadarMessage()
{
    return true;
}

ImageProvider *Client::imageProvider() const
{
    return m_imageProvider;
}
