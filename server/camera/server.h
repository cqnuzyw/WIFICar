#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QImage>

class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server(QObject *parent = nullptr);

	//initialize with config
	void init();

signals:
	void fatalError(QString error);
	void cameraStateChanged(bool open);

public slots:
	void sendImage(QImage image);

private slots:
	void establishClientConnection();
	void connectionLost();

private:
	void getConfig(quint16 &port);

	QTcpServer *m_server;
	bool m_isClientConnected;
	QTcpSocket *m_clientSocket;
};

#endif // SERVER_H
