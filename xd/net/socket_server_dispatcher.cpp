#include "socket_server_dispatcher.h"
#include "../base/log.h"
#include "socket_poll.h"
#include <stdio.h>
#include <errno.h>
#include <string.h>

#define XD_DEFAULT_SOCKET_WORKER_THREAD_NUM 5

XDSocketServerWorkThread::XDSocketServerWorkThread()
                        : thread_()
                        , pollFD_(-1)
                        , isStop_(true)
                        , owner_(NULL)
                        , setupEvent_(true)
{

}

XDSocketServerWorkThread::~XDSocketServerWorkThread()
{

}

void XDSocketServerWorkThread::setOwner(XDSocketServerDispatcher *owner)
{
    owner_ = owner;
}

void XDSocketServerWorkThread::setName(const char *name)
{
    thread_.setName(name);
}

bool XDSocketServerWorkThread::start()
{
    bool ret = true;
    pollFD_ = XDSocketPoll::create();
    if (XDSocketPoll::isInvalid(pollFD_)) {
        return false;
    }
    isStop_ = false;
    ret = thread_.start(this);
    if (ret) {
        // 设置分离
        setupEvent_.wait();
        thread_.detach();
    }
    XD_LOG_mdebug("[XDSocketServerWorkThread] Thread:%s setup", thread_.name().c_str());
    return ret;
}

bool XDSocketServerWorkThread::acceptNewConnect(XDSockFD fd, void *ud)
{
    if (XDSocketPoll::add(pollFD_, fd, ud)) {
        return false;
    }
    return true;
}

bool XDSocketServerWorkThread::handleReadMessage(XDSocketPoll::XDEvent *event)
{
    return false;
}

bool XDSocketServerWorkThread::handleSendMessage(XDSocketPoll::XDEvent *event)
{
    return false;
}

bool XDSocketServerWorkThread::closeSocket(XDSockFD fd)
{
     XDSocketPoll::del(pollFD_, fd);
    return false;
}

bool XDSocketServerWorkThread::run()
{
    setupEvent_.set();
    XDSocketPoll::XDEvent event[128];
    int32 num;
    int32 i;
    while (!isStop_) {
        num = XDSocketPoll::wait(pollFD_, event, 128);
        if (-1 == num) {
            if (EINTR == errno) {
                continue;
            }
            XD_LOG_merror("[XDSocketServerWorkThread] poll错误 error:%s", strerror(errno));
            continue;
        }
        for (i = 0; i < num; ++i) {
            if (event[i].read) {
                handleReadMessage(&event[i]);
            }
            if (event[i].write) {
                handleSendMessage(&event[i]);
            }
        }
    }
    isStop_ = true;
}

XDSocketServerDispatcher::XDSocketServerDispatcher()
                        : threadNum_(XD_DEFAULT_SOCKET_WORKER_THREAD_NUM)
                        , workerIndex_(0)
                        , tid_(XDBaseThread::getCurrentThreadID())
{
}

XDSocketServerDispatcher::XDSocketServerDispatcher(int32 threadNum)
                        : threadNum_(threadNum > 0 ? threadNum : 1)
                        , workerIndex_(0)
                        , tid_(XDBaseThread::getCurrentThreadID())
{
}

XDSocketServerDispatcher::~XDSocketServerDispatcher()
{
    if (workerThreads_) {
        delete []workerThreads_;
    }
}

bool XDSocketServerDispatcher::init()
{
    // new (int (*[10])());
    workerThreads_ = new XDSocketServerWorkThread[threadNum_];
    for (int32 i = 0; i < threadNum_; ++i) {
        char name[32];
        snprintf(name, 32, "%s%d", "Socket-worker-thread-", i);
        workerThreads_[i].setName(name);
        workerThreads_[i].setOwner(this);
    }
}

bool XDSocketServerDispatcher::run()
{
    for (int32 i = 0; i < threadNum_; ++i) {
        workerThreads_[i].start();
    }
}
