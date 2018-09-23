#include "serial.h"
#include "internalconnection.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	Serial serial;
	InternalConnection connection;

	QObject::connect(&connection, &InternalConnection::fatalError, &a, &QCoreApplication::quit, Qt::QueuedConnection);
	QObject::connect(&connection, &InternalConnection::changeDirection, &serial, &Serial::changeDirection);
	QObject::connect(&connection, &InternalConnection::changeRadarState, &serial, &Serial::changeRadarState);

	connection.init();
	serial.init();

	return a.exec();
}
