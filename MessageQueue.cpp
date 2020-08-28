#include "MessageQueue.h"

MessageQueue::MessageQueue(key_t key)
    : _key(key)
    , _msqid(-1)
{
}

MessageQueue::~MessageQueue()
{
    if (msgctl(_msqid, IPC_RMID, NULL) == -1) {
        ;
    }
}

bool MessageQueue::Initialize()
{
    if ((_msqid = msgget(_key, IPC_CREAT | 0666)) == -1) {
        return false;
    }
    return true;
}

bool MessageQueue::IsInitialize(MessageQueue* msgq)
{
    int size = 0;
    msgq->Size(size);
    if (size) {	// 이전에 사용된 큐에 메시지가 남아 있을 때
        msgq->~MessageQueue();
        if (!msgq->Initialize()) {
            return false;
        }
    }
    return (_msqid != -1);
}

bool MessageQueue::Push(MESSAGE* pMessage)
{
    if (msgsnd(_msqid, pMessage, sizeof(MESSAGE), 0) == -1) {
        return false;
    }
    return true;
}

bool MessageQueue::Pop(MESSAGE* pRetMessage)
{
    if (msgrcv(_msqid, pRetMessage, sizeof(MESSAGE), 0, 0) == -1) {
        return false;
    }
    return true;
}

bool MessageQueue::Size(int& size)
{
    if (msgctl(_msqid, IPC_STAT, &_stat) == -1) {
        size = 0;
        return false;
    }
    size = (int)_stat.msg_qnum;
    return true;
}

void MessageQueue::Print()
{
    if (msgctl(_msqid, IPC_STAT, &_stat) == -1) {
        printf(" message queue get status err \n");
        return;
    }

    printf("========== messege queue info =============\n");
    printf(" message queue info \n");
    printf(" msg_lspid : %d\n", _stat.msg_lspid);   // pid of last msgsnd
    printf(" msg_qnum : %lu\n", _stat.msg_qnum);    // number of current msg in queue
    printf(" msg_clen : %lu\n", _stat.msg_cbytes); // number of bytes of current queue
    printf(" msg_qlen : %lu\n", _stat.msg_qbytes); // max number of bytes on queue
    printf(" msg_stime : %lu\n", _stat.msg_stime);  // last msgsnd time
}

int MessageQueue::GetId()
{
    return _msqid;
}

int MessageQueue::GetKey()
{
    return _key;
}
