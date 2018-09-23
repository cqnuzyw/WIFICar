#include "internalconnection.h"
#include <string>
#include <iostream>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QFile>

using std::string;			using std::cout;
using std::cerr;			using std::endl;

InternalConnection::InternalConnection(QObject *parent)
	: QObject(parent)
{
	m_innerConnection = new QTcpSocket(this);
}

void InternalConnection::init()
{
	quint16 port = 0;
	getConfig(port);

	establishInternalConnection(port);
}

void InternalConnection::receiveMessage()
{
	//read
	QByteArray message;
	message = m_innerConnection->readAll();

	//analysis message
	QJsonParseError jsonError;
	QJsonDocument document = QJsonDocument::fromJson(message, &jsonError);
	if(!document.isNull() && (jsonError.error == QJsonParseError::NoError) && document.isObject())
	{
		QJsonObject object = document.object();
		if(object.contains("camera"))
		{
			QJsonValue value = object.value("camera");
			if(value.isBool())
			{
				bool state = value.toVariant().toBool();
				emit cameraStateChanged(state);
			}
		}
	}
	else
		cout << "Camera state ananlysis failed: " << string(message.data()) << endl;
}

void InternalConnection::connectionLost()
{
	if(qobject_cast<QTcpSocket *>(sender()) == m_innerConnection)
	{
		string error("Internal error: connection lost");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}
}

void InternalConnection::establishInternalConnection(quint16 port)
{
	m_innerConnection->connectToHost("127.0.0.1", port);
	if(m_innerConnection->waitForConnected(-1))
	{
		QJsonObject connectionMessage;
		connectionMessage.insert("camera", true);
		QJsonDocument document;
		document.setObject(connectionMessage);
		QByteArray messageArray = document.toJson(QJsonDocument::Compact);
		m_innerConnection->write(messageArray.data(), messageArray.size());

		connect(m_innerConnection, &QTcpSocket::readyRead, this, &InternalConnection::receiveMessage);
		connect(m_innerConnection, &QTcpSocket::disconnected, this, &InternalConnection::connectionLost);
		cout << "Established internal connection" << endl;
	}
	else
	{
		string error("Internal connect with control server failed");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}
}

void InternalConnection::getConfig(quint16 &port)
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
			QJsonValue control = object.value("control");
			if(control.isObject())
			{
				QJsonObject obj = control.toObject();
				QJsonValue value = obj.value("port");
				port = static_cast<quint16>(value.toVariant().toUInt());
			}
		}
	}
	else
	{
		string error("Initialize configure failed: internal connection with control");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}

	cout << "Camera internal config setted" << endl;
}
