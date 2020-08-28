#pragma once
#include <iostream>
#include <thread>
#include <unistd.h>
#include "MessageQueue.h"
#include "Camera.h"
#include "Plate.h"

using namespace std;

class AnprEV
{
public:
	AnprEV(key_t key);
	AnprEV(key_t key, string addr);
	virtual ~AnprEV();

public:
	void		Initialize();
	static void send(Camera* cam);
	static void recv(Camera* cam);
	static void plate(Plate* plate);
	void		Start();

private:
	Camera*	_cam;
	Camera* _cam1;
	Plate*	_plate;
};
