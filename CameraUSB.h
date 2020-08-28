#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

class CameraUSB
{
public:
	CameraUSB();
	virtual ~CameraUSB();

public:
	string GetCap();
	string GetOut();

private:
	string _usbCap;
	string _usbOut;
};

