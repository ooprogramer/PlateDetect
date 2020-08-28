#pragma once
#include <opencv2/opencv.hpp>
#include <iostream>
#include <string>

using namespace std;

class CameraIP
{
public:
	CameraIP(string addr);
	virtual ~CameraIP();

public:
	string GetCap();
	string GetOut();

private:
	string _ipCap;
	string _ipOut;
};

