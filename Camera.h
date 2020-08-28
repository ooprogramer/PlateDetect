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
	bool	InitializeCap();	// videocapture, videowriter 초기화
	bool	InitializeMsgq();	// camera의 message queue 객체 초기화
	void	SetSwitch();		// 스레드 종료 flag 값 변경
	int 	GetSwitch();
	void	Streaming();		// 프레임을 받아와서 메시지큐에 삽입 전송
	void	Play();				// 메시지큐에서 프레임을 빼와서 출력 

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

