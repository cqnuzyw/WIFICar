#include "server.h"
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QFile>
#include <iostream>
#include <string>

using std::cout;			using std::endl;
using std::string;			using std::cerr;

Server::Server(QObject *parent)
	: QObject(parent)
{
	m_server = new QTcpServer(this);
	m_internalCameraConnetcion = new QTcpSocket(this);
	m_internalSerialConnection = new QTcpSocket(this);
	m_clientSocket = new QTcpSocket(this);
}

void Server::init()
{
	quint16 port;
	getConfig(port);

	m_server->listen(QHostAddress::Any, port);
	connect(m_server, &QTcpServer::newConnection, this, &Server::establishClientConnection);
}

void Server::establishClientConnection()
{
	QTcpSocket *newConnection = m_server->nextPendingConnection();
	if(!m_internalCameraConnetcion->isOpen() || !m_internalSerialConnection->isOpen())
	{
		newConnection->waitForReadyRead(-1);
		QByteArray message = newConnection->readAll();
		analysisConnectionMessage(newConnection, message);
	}
	else
		m_clientSocket = newConnection;
	connect(newConnection, &QTcpSocket::readyRead, this, &Server::receiveMessage);
	connect(newConnection, &QTcpSocket::disconnected, this, &Server::connectionLost);
	cout << "Accept new connection" << endl;
}

void Server::receiveMessage()
{
	QTcpSocket *messageSender = qobject_cast<QTcpSocket *>(sender());
	QByteArray message = messageSender->readAll();
	//reply
	sendMessage(message);

	emit clientMessage(message);
}

void Server::connectionLost()
{
	if(qobject_cast<QTcpSocket *>(sender()) == m_internalCameraConnetcion ||
			qobject_cast<QTcpSocket *>(sender()) == m_internalSerialConnection)
	{
		string error("Internal error: connection lost");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}
	else if(qobject_cast<QTcpSocket *>(sender()) == m_clientSocket)
		cout << "Client disconnected" << endl;
}

void Server::sendMessage(QByteArray message)
{
	if(m_clientSocket->isOpen())
		m_clientSocket->write(message);
}

void Server::controlCamera(QByteArray message)
{
	if(m_internalCameraConnetcion->isOpen())
		m_internalCameraConnetcion->write(message);
}

void Server::controlSerial(QByteArray message)
{
	if(m_internalSerialConnection->isOpen())
		m_internalSerialConnection->write(message);
}

void Server::analysisConnectionMessage(QTcpSocket *newConnection, QByteArray message)
{
	//analysis conection message
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(message, &jsonError);
	if(!document.isNull() && (jsonError.error == QJsonParseError::NoError) && document.isObject())
	{
		QJsonObject object = document.object();
		if(object.contains("camera"))
		{
			QJsonValue value = object.value("camera");
			if(value.isBool() && value.toVariant().toBool() && !m_internalCameraConnetcion->isOpen())
			{
				m_internalCameraConnetcion = newConnection;
				cout << "Camera connection established" << endl;
			}
			else
				cout << "Camera connection failed" << endl;
		}
		else if(object.contains("serial"))
		{
			QJsonValue value = object.value("serial");
			if(value.isBool() && value.toVariant().toBool() && !m_internalSerialConnection->isOpen())
			{
				m_internalSerialConnection = newConnection;
				cout << "Serial connection established" << endl;
			}
			else
				cout << "Serial connection failed" << endl;
		}
	}
	else
		cout << "Connection message ananlysis failed: " << string(message.data()) << endl;
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
		if(object.contains("control"))
		{
			QJsonValue camera = object.value("control");
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
		string error("Initialize control configure failed");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}

	cout << "Control server config set done" << endl;
}
