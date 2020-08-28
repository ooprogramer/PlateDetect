#include <iostream>
#include "AnprEV.h"

using namespace std;

// 유틸리티성 객체
// 1.메시지큐 객체

// 행위를 하는 주체, 역할별로 정의
// 1.카메라 객체 : 카메라로부터 영상을 가져와서 메시지큐에 넣어주는 역할, 스트리밍도 역할
// 2.번호인식 수행 객체: cv::Mat 형태로 프레임영상을 입력받아 번호인식 결과 리턴 역할
// 주변기기 통신
// 3.전기차 번호인식 객체: 총괄매니저로 메시지큐를 수신하는 대상

int main() 
{
    key_t key = 12345;
    
    //AnprEV* pAnprEV = new AnprEV(key);
    AnprEV* pAnprEV = new AnprEV(key, "rtsp://admin:straffic1345@192.168.100.5/Straming/channels/1");
    pAnprEV->Initialize();
    pAnprEV->Start();
    
    /*.ini
    [CAMERA]
    Type=1
    Camera* pCamera = new IPCamera(");
    pCamera->SetRtspSrc("rtsp://admin:straffic1345@192.168.100.5/Straming/channels/1");
    Camera* pCamera = new USBCamera();
    */


    printf("exit\n");
    return 0;
}