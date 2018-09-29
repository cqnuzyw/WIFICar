#include "tcpsocket.h"

TcpSocket::TcpSocket(QObject *parent)
    :QTcpSocket{parent},m_totalSize{0}
{
}

bool TcpSocket::connectToServer(const QString &address, quint16 PORT)
{
    this->connectToHost(QHostAddress(address),PORT);
    return this->waitForConnected(TIMEOUT);
}

bool TcpSocket::disconnectFromServer()
{
    this->disconnectFromHost();
    if(this->state()==QAbstractSocket::UnconnectedState||this->waitForDisconnected(TIMEOUT)){
        return true;
    }
    return false;
}

qint64 TcpSocket::sendMessage(const QByteArray &message)
{
    if(!this->isValid()){
        return -1;
    }
    qint64 len=this->write(message,message.size());
    if(!this->waitForBytesWritten(TIMEOUT)){
        return -1;
    }
    return len;
}

QByteArray TcpSocket::receiveMessage(qint64 maxlen)
{
    if(!this->isValid()){
        return nullptr;
    }
    setTotalSize(maxlen);
    if(this->bytesAvailable()<totalSize()){
        return nullptr;
    };
    QByteArray message=this->read(totalSize());
    setTotalSize(0);
    return message;
}

qint64 TcpSocket::totalSize() const
{
    return m_totalSize;
}

void TcpSocket::setTotalSize(const qint64 &totalSize)
{
    m_totalSize = totalSize;
}
