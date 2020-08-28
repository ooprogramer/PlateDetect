#include "CameraIP.h"

CameraIP::CameraIP(string addr)
    : _ipCap("rtspsrc location=rtsp://admin:straffic1345@192.168.100.5/Straming/channels/1 \
        latency=200 ! application/x-rtp, media=video, encoding-name=H264 ! \
        rtph264depay ! h264parse ! omxh264dec ! video/x-raw(memory:NVMM) ! \
        nvvidconv ! video/x-raw, format=BGRx ! \
        videoconvert ! video/x-raw, format=BGR ! appsink")
    , _ipOut("appsrc ! videoconvert ! video/x-raw, format=BGRx ! \
        nvvidconv ! omxh264enc insert-sps-pps=true ! \
        h264parse ! rtph264pay pt=96 ! queue ! \
        application/x-rtp, media=video, encoding-name=H264 ! \
        udpsink host=192.168.100.255 port=5000 sync=false")
{
    string first = "rtspsrc location=";
    string second = "latency=200 ! application/x-rtp, media=video, encoding-name=H264 ! \
        rtph264depay ! h264parse ! omxh264dec ! video/x-raw(memory:NVMM) ! \
        nvvidconv ! video/x-raw, format=BGRx ! \
        videoconvert ! video/x-raw, format=BGR ! appsink";
    string capstr = first + addr + second;
    //_ipCap = capstr;
}

CameraIP::~CameraIP()
{
}

string CameraIP::GetCap()
{
	return _ipCap;
}

string CameraIP::GetOut()
{
    return _ipOut;
}
