#pragma once
#include <iostream>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <opencv2/opencv.hpp>

typedef struct _MESSAGE 
{
    long    index;
    long*   pData;
    //cv::Mat* pData;

    _MESSAGE()
    {
        index = 1;
        pData = NULL;
    }
} MESSAGE;

class MessageQueue
{
private:
    typedef struct msqid_ds Q_STAT;

public:
    MessageQueue(key_t key);
    virtual ~MessageQueue();

public:
    bool    Initialize();
    bool    IsInitialize(MessageQueue* msgq);
    bool    Push(MESSAGE* pMessage);    // 수신쪽에서 메모리 해제필요
    bool    Pop(MESSAGE* pRetMessage);
    bool    Size(int& size);
    void    Print();
    int     GetId();
    int     GetKey();

private:
    int     _key;
    int     _msqid;
    Q_STAT  _stat;
};