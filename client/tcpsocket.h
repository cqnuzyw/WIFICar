#ifndef TCPSOCKET_H
#define TCPSOCKET_H

#include <QtNetwork>
#include <QObject>
#include <QByteArray>

#define TIMEOUT 100

class TcpSocket:public QTcpSocket
{
public:
    TcpSocket(QObject *parent=nullptr);

    bool connectToServer(const QString &address,quint16 PORT);
    bool disconnectFromServer();

    qint64 sendMessage(const QByteArray &message);
    QByteArray receiveMessage(qint64 maxlen);
    qint64 totalSize() const;
    void setTotalSize(const qint64 &totalSize);
private:
    qint64 m_totalSize;
};

#endif // TCPSOCKET_H
