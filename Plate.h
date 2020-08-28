#pragma once
#include <iostream>
#include <sys/time.h>
#include <string>
#include <vector>
#include <map>
#include <dlfcn.h>
#include <opencv2/opencv.hpp>
#include "MessageQueue.h"
#include "STrafficANPREngineAPI.h"

using namespace std;

class Plate
{
public:
	Plate(key_t key);
	virtual ~Plate();

public:
	bool EngineStart();
	bool InitializeMsgq();
	void DetectPlate();
	void SetSwitch();
	int  GetSwitch();

private:
	STrafficANPREngineAPI*	_pEngineAPI;
	MessageQueue*			_msgq2;
	int						_switch;
};

