#include <iostream>
#include "AnprEV.h"

using namespace std;

// ��ƿ��Ƽ�� ��ü
// 1.�޽���ť ��ü

// ������ �ϴ� ��ü, ���Һ��� ����
// 1.ī�޶� ��ü : ī�޶�κ��� ������ �����ͼ� �޽���ť�� �־��ִ� ����, ��Ʈ���ֵ� ����
// 2.��ȣ�ν� ���� ��ü: cv::Mat ���·� �����ӿ����� �Է¹޾� ��ȣ�ν� ��� ���� ����
// �ֺ���� ���
// 3.������ ��ȣ�ν� ��ü: �Ѱ��Ŵ����� �޽���ť�� �����ϴ� ���

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