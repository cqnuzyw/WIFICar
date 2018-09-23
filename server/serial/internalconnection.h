#ifndef INTERNALCONNECTION_H
#define INTERNALCONNECTION_H

#include <QObject>
#include <QTcpSocket>

class InternalConnection : public QObject
{
	Q_OBJECT
public:
	explicit InternalConnection(QObject *parent = nullptr);

	void init();

signals:
	void changeDirection(double direction);
	void changeRadarState(bool radar);

	void fatalError(QString error);

public slots:
	void receiveMessage();
	void connectionLost();

private:
	void establishInternalConnection(quint16 port);
	void getConfig(quint16 &port);

	QTcpSocket *m_innerConnection;
};

#endif // INTERNALCONNECTION_H
