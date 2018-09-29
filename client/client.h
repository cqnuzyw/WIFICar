#ifndef CLIENT_H
#define CLIENT_H

#include <QImage>
#include <QMutex>
#include <QString>
#include "tcpsocket.h"
#include "imageprovider.h"

#define       Connect       0x0F00
#define       Camera        0x0F01
#define       Radar         0x0F02
#define       Message       0x0F03
#define       PhotoSend     0x0F04
#define       DirectSend    0x0F05

#define ControlPORT 1111
#define CameraPORT  2222
#define RadarPORT   3333

class Client : public QObject
{
    Q_OBJECT
public:
    Client(QObject *parent=nullptr);
    ~Client();

    Q_INVOKABLE bool connectToHost(const QString &address);
    Q_INVOKABLE bool disconnectFromHost();
    Q_INVOKABLE bool sendControlMessage(qint64 sign,qint64 flag) const;

    ImageProvider *imageProvider() const;

signals:
    void connectStateChanged(const bool flag);
    void cameraStateChanged(const bool flag);
    void radarStateChanged(const bool flag);
    void messageStateChanged(const bool flag);
    void callQmlRefeshImage();
private:
    bool receiveControlMessage();
    bool receiveCameraMessage();
    bool receiveRadarMessage();

    TcpSocket *_control;
    TcpSocket *_camera;
    TcpSocket *_radar;

    ImageProvider *m_imageProvider;
    qint64 _imageSize;

    clock_t _startTime;
    clock_t _endTime;
};

#endif // CLIENT_H
