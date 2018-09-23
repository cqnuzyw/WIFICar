#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>

class Server : public QObject
{
	Q_OBJECT
public:
	explicit Server(QObject *parent = nullptr);

	//initialize with config
	void init();

signals:
	void fatalError(QString error);
	void clientMessage(QByteArray message);

public slots:
	void establishClientConnection();
	void receiveMessage();
	void connectionLost();

	void sendMessage(QByteArray message);
	void controlSerial(QByteArray message);
	void controlCamera(QByteArray message);

private:
	void establishInternalConnections();
	void analysisConnectionMessage(QTcpSocket *newConnection, QByteArray message);
	void getConfig(quint16 &port);

	QTcpServer *m_server;

	QTcpSocket *m_internalCameraConnetcion;
	QTcpSocket *m_internalSerialConnection;
	QTcpSocket *m_clientSocket;
};

#endif // SERVER_H
