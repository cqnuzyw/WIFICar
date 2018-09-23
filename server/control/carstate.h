#ifndef CARSTATE_H
#define CARSTATE_H

#include <QObject>
#include <QString>

class CarState : public QObject
{
	Q_OBJECT
public:
	explicit CarState(QObject *parent = nullptr);

signals:
	void controlSerial(QByteArray message);
	void controlCamera(QByteArray message);

public slots:
	void updateCarState(QByteArray message);

private:
	bool power;
	bool radar;
	bool camera;
	double direction;
	/*            forward
	 *             1.57
	 * left 3.14    -1    0 right
	 *             4.71
	 *            backward
	 */
};

#endif // CARSTATE_H
