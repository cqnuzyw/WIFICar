#include "server.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QString>
#include <QFile>
#include <QBuffer>
#include <iostream>
#include <string>

using std::cerr;		using std::cout;
using std::endl;		using std::string;

Server::Server(QObject *parent)
	: QObject(parent), m_isClientConnected{false}
{
	m_server = new QTcpServer(this);
	m_clientSocket = new QTcpSocket(this);
}

void Server::init()
{
	quint16 port = 0;

	getConfig(port);

	//listen
	//allowed new connection
	m_server->listen(QHostAddress::Any, port);
	connect(m_server, &QTcpServer::newConnection, this, &Server::establishClientConnection);
}

void Server::establishClientConnection()
{
	m_clientSocket = m_server->nextPendingConnection();
	connect(m_clientSocket, &QTcpSocket::disconnected, this, &Server::connectionLost);
	m_isClientConnected = true;
	cout << "Accept new connection" << endl;
}

void Server::connectionLost()
{
	if(qobject_cast<QTcpSocket *>(sender()) == m_clientSocket)
	{
		m_isClientConnected = false;
		cout << "Disconnection with client" << endl;
		emit cameraStateChanged(false);
	}
}

void Server::sendImage(QImage image)
{
	if(m_isClientConnected)
	{
		QByteArray byte;
		QBuffer buf(&byte);
		image.save(&buf,"JPEG");
		QByteArray ss=qCompress(byte,1);
		QByteArray vv=ss.toBase64();

		QByteArray ba;
		QDataStream out(&ba,QIODevice::WriteOnly);
		out.setVersion(QDataStream::Qt_5_11);

		out << static_cast<quint64>(0);
        out << vv;
		out.device()->seek(0);
        out << static_cast<quint64>(static_cast<quint64>(ba.size()) - sizeof(quint64));
        m_clientSocket->write(ba);
	}
}

void Server::getConfig(quint16 &port)
{
	//get configs
	QFile configFile("./config.json");
	if(!configFile.open(QIODevice::ReadOnly))
	{
		string error("File config.json does not exist, failed to start");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}
	QByteArray config = configFile.readAll();
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(config, &jsonError);
	if(!document.isNull() && (jsonError.error == QJsonParseError::NoError) && document.isObject())
	{
		QJsonObject object = document.object();
		if(object.contains("camera"))
		{
			QJsonValue camera = object.value("camera");
			if(camera.isObject())
			{
				QJsonObject obj = camera.toObject();
				QJsonValue value = obj.value("port");
				port = static_cast<quint16>(value.toVariant().toUInt());
			}
		}
	}
	else
	{
		string error("Initialize configure failed");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}

	cout << "Server config set done" << endl;
}
