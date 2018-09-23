#include "camera.h"
#include <QFile>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonDocument>
#include <QVariant>
#include <iostream>
#include <string>

using std::cerr;		using std::cout;
using std::endl;		using std::string;
using namespace cv;

Camera::Camera(QObject *parent)
	: QObject(parent)
{
	m_timer = new QTimer(this);
}

Camera::~Camera()
{
	delete m_timer;
}

void Camera::init()
{
	int index = 0;
	int width = 0;
	int height = 0;
	int msecCaptureInterval = 0;

	getConfig(index, width, height, msecCaptureInterval);

	m_capture.open(index);
	if(!m_capture.isOpened())
	{
		string error("Cannot open camera");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}

	m_capture.set(CV_CAP_PROP_FRAME_WIDTH, width);
	m_capture.set(CV_CAP_PROP_FRAME_HEIGHT, height);
	m_timer->setInterval(msecCaptureInterval);

	connect(m_timer, &QTimer::timeout, this, &Camera::needToCaptureImage);
}

void Camera::cameraStateChanged(bool isOpen)
{
	if(isOpen)
		startCapture();
	else
		stopCapture();
}

void Camera::startCapture()
{
	cout << "Start camera" << endl;
	m_timer->start();
}

void Camera::stopCapture()
{
	cout << "Stop camera" << endl;
	m_timer->stop();
}

void Camera::needToCaptureImage()
{
	Mat frame;
	m_capture >> frame;
	cvtColor(frame, frame, CV_BGR2RGB);

	QImage image(static_cast<uchar *>(frame.data), frame.cols, frame.rows, QImage::Format_RGB888);
	emit capturedImage(image);
}

void Camera::getConfig(int &index, int &width, int &height, int &msecCaptureInterval)
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
		if(object.contains("camera"))
		{
			QJsonValue camera = object.value("camera");
			if(camera.isObject())
			{
				QJsonObject obj = camera.toObject();
				QJsonValue value = obj.value("cameraIndex");
				index = value.toVariant().toInt();
				value = obj.value("imageWidth");
				width = value.toVariant().toInt();
				value = obj.value("iamgeHeight");
				height = value.toVariant().toInt();
				value = obj.value("captureInterval");
				msecCaptureInterval = value.toVariant().toInt();
			}
		}
	}
	else
	{
		string error("Initialize camera configure failed");
		cerr << error << endl;
		emit fatalError(QString::fromStdString(error));
		return;
	}

	cout << "Camera Config setted" << endl;
}
