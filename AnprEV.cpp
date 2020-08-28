#include "AnprEV.h"

AnprEV::AnprEV(key_t key)
{
	_cam = new Camera(key);
	_plate = new Plate(key);
}

AnprEV::AnprEV(key_t key, string addr)
{
	_cam = new Camera(key, addr);
	_plate = new Plate(key);
}

AnprEV::~AnprEV()
{
}

void AnprEV::Initialize()
{
	if (!_plate->EngineStart()) {
		return;
	}
	if (!_plate->InitializeMsgq()) {
		printf("Plate message queue initialization failed\n");
		return;
	}
	if (!_cam->InitializeCap()) {
		printf("cannot open IP camera\n");
		return;
	}
	if (!_cam->InitializeMsgq()) {
		printf("Camera message queue initialization failed\n");
		return;
	}
}

void AnprEV::send(Camera* cam)
{
	cam->Streaming();
}

void AnprEV::recv(Camera* cam)
{
	cam->Play();
}

void AnprEV::plate(Plate* plate)
{
	plate->DetectPlate();
}

void AnprEV::Start()
{
	thread t2(recv, _cam);
	thread t1(send, _cam);
	thread t3(plate, _plate);
	while (true) {
		if (_plate->GetSwitch()) {
			_cam->SetSwitch();
			break;
		}
		usleep(30000);
	}
	t3.join();
	printf("plate join\n");
	t2.join();
	printf("recv join\n");
	t1.join();
	printf("send join\n");
	sleep(1);

	_cam->~Camera();
	return;
}
