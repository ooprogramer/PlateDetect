#include "CameraUSB.h"

CameraUSB::CameraUSB()
    : _usbCap("nvarguscamerasrc ! video/x-raw(memory:NVMM), \
        width=1280, height=720, format=NV12, framerate=30/1 ! \
        nvvidconv ! video/x-raw, format=BGRx ! \
        videoconvert ! video/x-raw, format=BGR ! appsink")
    , _usbOut("appsrc ! videoconvert ! x264enc tune=zerolatency \
        bitrate=5000 speed-preset=superfast ! rtph264pay ! \
        udpsink host=192.168.100.255 port=5000")
{
}

CameraUSB::~CameraUSB()
{
}

string CameraUSB::GetCap()
{
	return _usbCap;
}

string CameraUSB::GetOut()
{
	return _usbOut;
}
