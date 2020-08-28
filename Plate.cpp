#include "Plate.h"

Plate::Plate(key_t key)
	:	_switch(0)
{
	_msgq2 = new MessageQueue(key-1);
}

Plate::~Plate()
{
}

bool Plate::EngineStart()
{
	// get current path
	char cwd[1024] = { 0, };
	getcwd(cwd, sizeof(cwd));
	printf("Current working dir: %s\n", cwd);

	// load shared library object (Engine)
	char soFilePath[1024] = { 0, };
	snprintf(soFilePath, sizeof(soFilePath), "%s/STrafficANPREngineAPI.so", cwd);
	void* api = dlopen(soFilePath, RTLD_NOW);
	if (api == NULL)
	{
		printf(".so load err: %s\n", soFilePath);
		return false;
	}

	// load engine factory function
	char* error = NULL;
	CreateANPREngineAPI* CreateEngine = (CreateANPREngineAPI*)dlsym(api, "ENGINE_CREATE");
	if ((error = dlerror()) != NULL)
	{
		printf("ERR: %s\n", error);
		exit(0);
	}

	// create engine
	_pEngineAPI = CreateEngine();
	printf("STraffic ANPR Engine Version is %s\n", _pEngineAPI->GetVersion());

	// initialize engine
	STrafficError ret = _pEngineAPI->Initialize();
	if (ret != STrafficError::OK)
	{
		printf("Failed to initialize engine: %s", _pEngineAPI->GetSTrafficError(ret));
		return false;
	}
	dlclose(api);
	return true;
}

bool Plate::InitializeMsgq()
{
	if (!_msgq2->Initialize()) {
		return false;
	}
	_msgq2->IsInitialize(_msgq2);
	return true;
}

void Plate::DetectPlate()
{
	MESSAGE msg;
	cv::Mat* pImg;
	int qCurSize;

	double prev = static_cast<double>(cv::getTickCount());
	double now;

	long long init = 0;
	int fps_tick = 0;
	int cnt = 0;
	struct timeval te;

	while (true) {
		_msgq2->Size(qCurSize);
		//printf("----------Queue2 %d count\n", qCurSize);
		for (int i = 0; i < qCurSize - 1; i++) {
			_msgq2->Pop(&msg);
			//printf("          Queue2 %ld remove\n", msg.index);
			delete (cv::Mat*)msg.pData;
		}
		//printf("          Queue2 %ld recv\n", msg.index);
		_msgq2->Pop(&msg);
		pImg = (cv::Mat*)msg.pData;

		gettimeofday(&te, NULL);
		if (!init) {
			init = te.tv_sec;
		}
		else {
			if (init == te.tv_sec) {
				cnt++;
			}
			else {
				fps_tick = cnt;
				cnt = 0;
				init = te.tv_sec;
			}
		}

		// --> ANPR Recognition Start
		std::vector<LPN_RESULT> results;
		STrafficError err = _pEngineAPI->Recognize(*pImg, results, true);

		if (err != STrafficError::OK) {
			printf("\nrecog error: %s", _pEngineAPI->GetSTrafficError(err));
		}
		else {
			printf("\nrecog count: %d", (int)results.size());
			if (results.empty()) {
				printf("\nRecog fail: No plate\n");
			}
			for (auto& result : results) {
				printf("\nRecog result: %s\n", result.lpn);
			}
		}
		// --> ANPR Recognition End

		now = static_cast<double>(cv::getTickCount());
		std::string text = "FPS: " + to_string(fps_tick) +
			"  oneLoop: " + to_string(int((now - prev) / cv::getTickFrequency() * 1000000));
		cv::putText(*pImg, text, cv::Point(10, 40), 1, 2, cv::Scalar(255, 0, 255), 2.5);


		cv::imshow("msgq2", *pImg);
		int keycode = cv::waitKey(1) & 0xff;
		if (keycode == 'q') {
			SetSwitch();
			break;
		}

		prev = now;

		delete pImg;
	}
	printf("Queue2 recv done\n");
	cv::destroyAllWindows();
}

void Plate::SetSwitch()
{
	_switch = 1;
}

int Plate::GetSwitch()
{
	return _switch;
}
