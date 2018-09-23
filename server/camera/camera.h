#ifndef CAMERA_H
#define CAMERA_H

#include <QObject>
#include <QImage>
#include <QTimer>
#include <QString>
#include <opencv2/opencv.hpp>

class Camera : public QObject {
	Q_OBJECT
public:
	explicit Camera(QObject *parent = nullptr);
	~Camera();

	void init();

signals:
	void capturedImage(QImage capturedImage);
	void fatalError(QString error);

public slots:
	void cameraStateChanged(bool isOpen);
	void startCapture();
	void stopCapture();

private slots:
	void needToCaptureImage();
	
private:
	void getConfig(int &index, int &width, int &height, int &msecCaptureInterval);
	cv::VideoCapture m_capture;
	QTimer *m_timer;
};

#endif // CAMERA_H
