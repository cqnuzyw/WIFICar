#ifndef SERIAL_H
#define SERIAL_H

#include <QObject>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>

class Serial : public QObject
{
	Q_OBJECT
public:
	explicit Serial(QObject *parent = nullptr);

	void init();

	void writeDataToSerial(QByteArray data);

signals:
	void fatalError(QString error);

public slots:
	void changeDirection(double direction);
	void changeRadarState(bool isOpen);

	void readSerialData();

private:
	void getConfig(QString &portName, qint32 &baudRate, QSerialPort::DataBits &dataBits, QSerialPort::Parity &parity, QSerialPort::StopBits &stopBits);
	QByteArray QString2Hex(QString str);
	char char2Hex(char c);

	QSerialPort *m_serial;

	//commands
	QByteArray m_forward;
	QByteArray m_backward;
	QByteArray m_turnLeft;
	QByteArray m_turnRight;
	QByteArray m_stop;
};

#endif // SERIAL_H
