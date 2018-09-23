#include "server.h"
#include "carstate.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	Server ser;
	CarState state;

	QObject::connect(&ser, &Server::fatalError, &a, &QCoreApplication::quit, Qt::QueuedConnection);
	QObject::connect(&ser, &Server::clientMessage, &state, &CarState::updateCarState);
	QObject::connect(&state, &CarState::controlSerial, &ser, &Server::controlSerial);
	QObject::connect(&state, &CarState::controlCamera, &ser, &Server::controlCamera);

	ser.init();

	return a.exec();
}
