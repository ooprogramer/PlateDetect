#pragma once
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <string>
#include <opencv2/opencv.hpp>
#include "MessageQueue.h"
#include "CameraIP.h"
#include "CameraUSB.h"

using namespace std;

class Camera
{
public:
	Camera(key_t key);
	Camera(key_t key, string addr);
	virtual ~Camera();

public:
	bool	InitializeCap();	// videocapture, videowriter �ʱ�ȭ
	bool	InitializeMsgq();	// camera�� message queue ��ü �ʱ�ȭ
	void	SetSwitch();		// ������ ���� flag �� ����
	int 	GetSwitch();
	void	Streaming();		// �������� �޾ƿͼ� �޽���ť�� ���� ����
	void	Play();				// �޽���ť���� �������� ���ͼ� ��� 

private:
	CameraIP*				_ipCam;
	CameraUSB*				_usbCam;
	string					_capArg;
	string					_outArg;
	cv::VideoCapture		_cap;
	cv::VideoWriter			_out;
	cv::Size				_size;
	double					_fps;
	MessageQueue*			_msgq1;
	MessageQueue*			_msgq2;
	int						_switch;
};

