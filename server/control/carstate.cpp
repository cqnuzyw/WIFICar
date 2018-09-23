#include "carstate.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <QByteArray>
#include <QDataStream>

CarState::CarState(QObject *parent)
	: QObject(parent), power{false}, radar{false}, camera{false}, direction{-1}
{}

void CarState::updateCarState(QByteArray message)
{
	QDataStream msgStream(message);
	msgStream.setVersion(QDataStream::Qt_5_11);
	qint64 sign, state;
	msgStream >> sign >> state;

	switch(sign)
	{
		case 0x0F00: break;
		case 0x0F01:
			camera = (state == 1);
			break;
		case 0x0F02:
			radar = (state == 1);
			break;
		case 0x0F03:
			//message
			break;
		case 0x0F04:
			//photo send
			break;
		case 0x0F05:
			direction = state;
			break;
	}

	//camera
	QJsonObject cameraObject;
	cameraObject.insert("camera", camera);
	QJsonDocument cameraDocument;
	cameraDocument.setObject(cameraObject);
	QByteArray cameraMessage = cameraDocument.toJson(QJsonDocument::Compact);
	emit controlCamera(cameraMessage);

	//serial
	QJsonObject serialObject;
	serialObject.insert("direction", direction);
	serialObject.insert("radar", radar);
	QJsonDocument serialDocument;
	serialDocument.setObject(serialObject);
	QByteArray serialMessage = serialDocument.toJson(QJsonDocument::Compact);
	emit controlSerial(serialMessage);
}
