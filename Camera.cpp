#include "Camera.h"

Camera::Camera(key_t key)
{
	_msgq1 = new MessageQueue(key);
	_msgq2 = new MessageQueue(key-1);
	_usbCam = new CameraUSB();
	_capArg = _usbCam->GetCap();
	_outArg = _usbCam->GetOut();
	_switch = 0;
}

Camera::Camera(key_t key, string addr)
{
	_msgq1 = new MessageQueue(key);
	_msgq2 = new MessageQueue(key-1);
	_ipCam = new CameraIP(addr);
	_capArg = _ipCam->GetCap();
	_outArg = _ipCam->GetOut();
	_switch = 0;
}

Camera::~Camera()
{
	_msgq1->~MessageQueue();
	_msgq2->~MessageQueue();
}

bool Camera::InitializeCap()
{
	_cap.open(_capArg, cv::CAP_GSTREAMER);
	_size = cv::Size((int)_cap.get(cv::CAP_PROP_FRAME_WIDTH),
		(int)_cap.get(cv::CAP_PROP_FRAME_HEIGHT));
	_fps = _cap.get(cv::CAP_PROP_FPS);
	_out.open(_outArg, cv::CAP_GSTREAMER, 0, _fps, _size, true);
	if (!_cap.isOpened() || !_out.isOpened()) {
		return false;
	}
	return true;
}

bool Camera::InitializeMsgq()
{
	if (!_msgq1->Initialize()) {
		return false;
	}
	if (!_msgq2->Initialize()) {
		return false;
	}
	_msgq1->IsInitialize(_msgq1);
	//_msgq2->IsInitialize(_msgq2);
	return true;
}

void Camera::SetSwitch()
{
	_switch = 1;
}

int Camera::GetSwitch()
{
	return _switch;
}

void Camera::Streaming()
{
	MESSAGE msg;
    while (true) {
		cv::Mat* pImg = new cv::Mat();
		_cap.read(*pImg);
		msg.index++;
		msg.pData = (long*)pImg;
		if (GetSwitch()) {
			break;
		}
		_msgq1->Push(&msg);
		//printf("%ld sent\n", msg.index);
	}
    printf("send done\n");
	_cap.release();
	return;
}

void Camera::Play()
{
	MESSAGE msg;
	cv::Mat* pImg;
	int qCurSize;
	int frame=0;
	while (true) {
		_msgq1->Size(qCurSize);
		//printf("Queue1 %d count\n", qCurSize);
		for (int i = 0; i < qCurSize - 1; i++) {
			_msgq1->Pop(&msg);
			//printf("Queue1 %ld remove\n", msg.index);
			delete (cv::Mat*)msg.pData;
		}
		//printf("Queue1 %ld recv\n", msg.index);
		_msgq1->Pop(&msg);
		pImg = (cv::Mat*)msg.pData;

		/*차량 객체 인식 --> 모든 프레임*/
		usleep(50000);
		// 모든 프레임 write하고 flag를 지정해서 다음 큐로 보내거나 지우거나

		_out.write(*pImg);

		if (GetSwitch()) {
			break;
		}

		frame++;
		printf("frame number:%d\n", frame);
		if(frame < 500){
			_msgq2->Push(&msg);
		}
		else {
			delete pImg;
		}

		if (frame > 1000) {
			frame = 0;
		}
	}
	printf("Queue1 recv done\n");
	_out.release();
	return;
}

//int id = _msgq2->GetId(); printf("camera ID:%d\n", id);
//int key = _msgq2->GetKey(); printf("camera key:%d\n", key);