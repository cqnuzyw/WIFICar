#include "serial.h"
#include <QByteArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QFile>
#include <string>
#include <iostream>

using std::string;		using std::cerr;
using std::endl;		using std::cout;

Serial::Serial(QObject *parent)
	: QObject(parent)
{
	m_serial = new QSerialPort;
}

void Serial::init()
{
	QString portName;
	qint32 baudRate;
	QSerialPort::DataBits dataBits;
	QSerialPort::Parity parity;
	QSerialPort::StopBits stopBits;

	getConfig(portName, baudRate, dataBits, parity, stopBits);

	m_serial->setPortName(portName);
	if(!m_serial->open(QIODevice::ReadWrite))
	{
		string error("Can not open serial port");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}
	m_serial->setBaudRate(baudRate);
	m_serial->setDataBits(dataBits);
	m_serial->setParity(parity);
	m_serial->setStopBits(stopBits);

	connect(m_serial, &QSerialPort::readyRead, this, &Serial::readSerialData);
}

void Serial::writeDataToSerial(QByteArray data)
{
	m_serial->write(data.data(), data.size());
}

void Serial::changeDirection(double direction)
{
}

void Serial::changeRadarState(bool isOpen)
{
}

void Serial::readSerialData()
{
	QByteArray data = m_serial->readAll();
}

void Serial::getConfig(QString &portName, qint32 &baudRate, QSerialPort::DataBits &dataBits, QSerialPort::Parity &parity, QSerialPort::StopBits &stopBits)
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
		if(object.contains("serial"))
		{
			QJsonValue serial = object.value("serial");
			if(serial.isObject())
			{
				QJsonObject serialObj = serial.toObject();
				//port setting
				QJsonValue serialValue = serialObj.value("portName");
				portName = serialValue.toString();
				serialValue = serialObj.value("baudRate");
				int baudRateNum = serialValue.toInt();
				switch(baudRateNum)
				{
					case 1200: baudRate = QSerialPort::Baud1200;
						break;
					case 2400: baudRate = QSerialPort::Baud2400;
						break;
					case 4800: baudRate = QSerialPort::Baud4800;
						break;
					case 9600: baudRate = QSerialPort::Baud9600;
						break;
					case 19200: baudRate = QSerialPort::Baud19200;
						break;
					case 38400: baudRate = QSerialPort::Baud38400;
						break;
					case 57600: baudRate = QSerialPort::Baud57600;
						break;
					case 115200: baudRate = QSerialPort::Baud115200;
						break;
				}
				serialValue = serialObj.value("dataBits");
				int dataBitsNum = serialValue.toInt();
				switch(dataBitsNum)
				{
					case 5: dataBits = QSerialPort::Data5;
						break;
					case 6: dataBits = QSerialPort::Data6;
						break;
					case 7: dataBits = QSerialPort::Data7;
						break;
					case 8: dataBits = QSerialPort::Data8;
						break;
				}
				serialValue = serialObj.value("parity");
				int parityNum = serialValue.toInt();
				switch(parityNum)
				{
					case 'n': parity = QSerialPort::NoParity;
						break;
					case 'e': parity = QSerialPort::EvenParity;
						break;
					case 'o': parity = QSerialPort::OddParity;
						break;
					case 's': parity = QSerialPort::SpaceParity;
						break;
					case 'm': parity = QSerialPort::MarkParity;
						break;
				}
				serialValue = serialObj.value("stopBits");
				int stopBitsNum = serialValue.toInt();
				switch(stopBitsNum)
				{
					case 1: stopBits = QSerialPort::OneStop;
						break;
					case 2: stopBits = QSerialPort::OneAndHalfStop;
						break;
					case 3: stopBits = QSerialPort::TwoStop;
						break;
				}
				serialValue = serialObj.value("command");
				if(serialValue.isObject())
				{
					QJsonObject commandObj = serialValue.toObject();
					QJsonValue commandValue = commandObj.value("forward");
					m_forward = QString2Hex(commandValue.toString());
					commandValue = commandObj.value("backward");
					m_backward = QString2Hex(commandValue.toString());
					commandValue = commandObj.value("turnleft");
					m_turnLeft = QString2Hex(commandValue.toString());
					commandValue = commandObj.value("turnright");
					m_turnRight = QString2Hex(commandValue.toString());
					commandValue = commandObj.value("stop");
					m_stop = QString2Hex(commandValue.toString());
				}
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

QByteArray Serial::QString2Hex(QString str)
{
	QByteArray data;

	int length = str.length();
	for(int i = 2; i < length; i += 2)		//skip 0x
	{
		if(i + 1 < length)
		{
			char c = str[i].toLatin1();
			char hex = 0;

			hex = char2Hex(c) * 16;
			c = str[i + 1].toLatin1();
			hex += char2Hex(c);

			data.push_back(hex);
		}
	}

	return data;
}

char Serial::char2Hex(char c)
{
	char hex = 0;
	if(c >= '0' && c <= '9')
		hex = c - '0';
	else if(c >= 'A' && c <= 'F')
		hex = c - 'A' + 10;
	else if(c >= 'a' && c <= 'f')
		hex = c - 'a' + 10;

	return hex;
}
