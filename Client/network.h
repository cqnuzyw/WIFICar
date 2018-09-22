#ifndef NETWORK_H
#define NETWORK_H

#define       Connect       0x0F00
#define       Camera        0x0F01
#define       Radar         0x0F02
#define       Message       0x0F03
#define       PhotoSend     0x0F04
#define       DirectSend    0x0F05

#define ControlPORT 1111
#define CameraPORT 2222
#define IP "10.42.0.1"
#define TIMEOUT 1000

#include <QObject>
#include <QtNetwork>
#include <QImage>
#include <QQuickImageProvider>
#include <QMutex>

class ImageProvider : public QQuickImageProvider
{
public:
    ImageProvider();
    QImage requestImage(const QString &id, QSize *size, const QSize &requestedSize);
    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);
    QImage img;
};

class NetWork:public QObject
{
    Q_OBJECT
public:
    NetWork(QObject *parent=nullptr);
    Q_INVOKABLE void connectServer();
    Q_INVOKABLE void disconnectServer();
    ImageProvider *pImgProvider() const;

signals:
    void connectStateChanged(const int flag);
    void cameraStateChanged(const bool flag);
    void radarStateChanged(const bool flag);
    void messageStateChanged(const bool flag);

    void cameraChanged(const bool flag);
    void radarChanged(const bool flag);
    void messageChanged(const bool flag);
    void directionChanged(const double direction);

    void callQmlRefeshImage();
private slots:
    void socketStateChanged(QAbstractSocket::SocketState socketState);
    bool receiveControlMessage();
    bool receiveCameraMessage();

    void setImage(QImage image);
private:
    bool sendMessage(qint64 sign, qint64 length);

    QTcpSocket *_control;
    QTcpSocket *_camera;
    ImageProvider *m_pImgProvider;

    qint64 totalBytes;      //一个数据包MSG部分的完整大小
    qint64 recvdBytes;      //已经收到的字节数
    qint64 serverCmd;       //接受数据包的类型
    QByteArray inBlock;     //接受缓冲
    QByteArray m_buffer;    //缓存上一次或多次的未处理的数据
    QMutex mutex;
    quint64 totalSize;

    clock_t startTime;
    clock_t endTime;
};

#endif // NETWORK_H
