#include "network.h"
#include <vector>
#include <QImage>
#include <iostream>
#include <QByteArray>

using std::vector;

NetWork::NetWork(QObject *parent)
    :QObject{parent}
{
    _control=new QTcpSocket;
    _camera=new QTcpSocket;
    m_pImgProvider=new ImageProvider;
    totalSize=0;
    connect(this,&NetWork::cameraChanged,[&](const bool flag){
        if(!sendMessage(Camera,flag))
            emit cameraStateChanged(false);
    });
    connect(this,&NetWork::messageChanged,[&](const bool flag){
        if(!sendMessage(Message,flag))
            emit messageStateChanged(false);
    });
    connect(this,&NetWork::radarChanged,[&](const bool flag){
        if(!sendMessage(Radar,flag))
            emit radarStateChanged(false);
    });
    connect(this,&NetWork::directionChanged,[&](const double direction){
        sendMessage(static_cast<qint64>(DirectSend),static_cast<qint64>(direction));
    });
    connect(_control,&QTcpSocket::stateChanged,this,[&](QAbstractSocket::SocketState socketState){
        socketStateChanged(socketState);
    });
    connect(_camera,&QTcpSocket::stateChanged,this,[&](QAbstractSocket::SocketState socketState){
        socketStateChanged(socketState);
    });
    connect(_control,&QTcpSocket::readyRead,this,&NetWork::receiveControlMessage);
    connect(_camera,&QTcpSocket::readyRead,this,&NetWork::receiveCameraMessage);
}

void NetWork::connectServer()
{
    _control->connectToHost(QHostAddress(IP),ControlPORT);
    _control->waitForConnected(TIMEOUT);
    _camera->connectToHost(QHostAddress(IP),CameraPORT);
    _camera->waitForConnected(TIMEOUT);
}

void NetWork::disconnectServer()
{
    if(_control->isValid()){
        _control->disconnectFromHost();
        if(_control->state()!=QAbstractSocket::UnconnectedState)
            _control->waitForDisconnected(TIMEOUT);
    }
    if(_camera->isValid()){
        _camera->disconnectFromHost();
        if(_camera->state()!=QAbstractSocket::UnconnectedState)
            _camera->waitForDisconnected(TIMEOUT);
    }
}

bool NetWork::sendMessage(qint64 sign,qint64 length)
{
    if(!_control->isValid()){
        return false;
    }
    QByteArray block;
    QDataStream output(&block,QIODevice::WriteOnly);
    output.setVersion(QDataStream::Qt_5_11);

    output<<qint64(sign)<<qint64(length);
    _control->write(block,block.size());
    block.resize(0);
    return true;
}

ImageProvider *NetWork::pImgProvider() const
{
    return m_pImgProvider;
}

bool NetWork::receiveControlMessage()
{
    QByteArray block;
    block=_control->read(sizeof(qint64)*2);
    QDataStream output(&block,QIODevice::ReadOnly);
    output.setVersion(QDataStream::Qt_5_2);

    qint64 sign,length;
    output>>sign>>length;
    block.resize(0);
    return true;
}

bool NetWork::receiveCameraMessage()
{
    if(totalSize==0){
        while(_camera->bytesAvailable()<static_cast<qint64>(sizeof(quint64)));
        QDataStream in(_camera);
        in.setVersion(QDataStream::Qt_5_11);
        in>>totalSize;
        startTime=clock();
    }
    if(totalSize>0){
        if(_camera->bytesAvailable()<static_cast<qint64>(totalSize)){
            return true;
        }
        QByteArray message;
        QDataStream in(_camera);
        in.setVersion(QDataStream::Qt_5_11);
        in>>message;
        //message=_camera->read(static_cast<qint64>(totalSize));
        QString ss=QString::fromLatin1(message.data(),message.size());
        QByteArray rc;
        rc=QByteArray::fromBase64(ss.toLatin1());
        QByteArray rdc=qUncompress(rc);
        QImage imageC;
        bool flag=imageC.loadFromData(rdc,"JPEG");
        if(flag){
            setImage(imageC);
        }
        else{
            std::cout<<"F"<<std::endl;
        }
        endTime=clock();
        std::cout<<static_cast<double>(1)/static_cast<double>((endTime-startTime)/1000)<<" FPS"
                <<"\t"
               <<static_cast<double>(totalSize)/static_cast<double>(1024)<<"KB"
              <<std::endl;
        totalSize=0;
    }
    return true;
}

void NetWork::setImage(QImage image)
{
    m_pImgProvider->img=image;
    emit callQmlRefeshImage();
}

void NetWork::socketStateChanged(QAbstractSocket::SocketState socketState)
{
    int flag=0;
    switch(socketState){
    case QAbstractSocket::UnconnectedState:
        flag=0;break;
    case QAbstractSocket::HostLookupState:
        flag=1;break;
    case QAbstractSocket::ConnectingState:
        flag=2;break;
    case QAbstractSocket::ConnectedState:
        flag=3;break;
    case QAbstractSocket::BoundState:
        flag=4;break;
    case QAbstractSocket::ClosingState:
        flag=5;break;
    case QAbstractSocket::ListeningState:
        flag=6;break;
    }
    emit connectStateChanged(flag);
}

ImageProvider::ImageProvider()
    : QQuickImageProvider(QQuickImageProvider::Image)
{
}

QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    return this->img;
}

QPixmap ImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    return QPixmap::fromImage(this->img);
}
