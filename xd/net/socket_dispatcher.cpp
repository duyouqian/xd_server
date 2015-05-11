#include "socket_dispatcher.h"
#include "../base/guard_mutex.h"
#include <string>

class XDSocketWorkThread : public XDBaseObject
{
public:
    XDSocketWorkThread();
    ~XDSocketWorkThread();

    void setName(const char *name);

    bool add(XDSockFD fd, void *ud);
    bool del(XDSockFD fd);
    bool write(XDSockFD fd, void *ud, bool enable);

    bool start();
    bool stop();

private:
    XDEventLoop eventLoop_;
    std::string name_;
};

XDSocketWorkThread::XDSocketWorkThread()
                  : eventLoop_()
                  , name_()
{

}

XDSocketWorkThread::~XDSocketWorkThread()
{
    stop();
}

void XDSocketWorkThread::setName(const char *name)
{
    name_.assign(name);
    eventLoop_.setName(name);
}

FROCEINLINE bool XDSocketWorkThread::add(XDSockFD fd, void *ud)
{
    return eventLoop_.add(fd, ud);
}

FROCEINLINE bool XDSocketWorkThread::del(XDSockFD fd)
{
    return eventLoop_.del(fd);
}

FROCEINLINE bool XDSocketWorkThread::write(XDSockFD fd, void *ud, bool enable)
{
    return eventLoop_.write(fd, ud, enable);
}

bool XDSocketWorkThread::start()
{
    if (!eventLoop_.init()) {
        return false;
    }
    if (!eventLoop_.start()) {
        return false;
    }
    return true;
}

bool XDSocketWorkThread::stop()
{
    eventLoop_.stop();
    eventLoop_.wakeup();
    return true;
}

XDSocketDispather::XDSocketDispather(int32 threadNum)
                 : mainEventLoop_("SocketDispatherMainEventLoop")
                 , threadNum_(threadNum > 1 ? threadNum - 1 : 0)
                 , workerThreads_(NULL)
                 , workerIndex_(0)
{

}

XDSocketDispather::~XDSocketDispather()
{
    stop();
}

bool XDSocketDispather::start()
{
    if (threadNum_ > 0) {
        workerThreads_ = new XDSocketWorkThread[threadNum_];
        check(workerThreads_);
        for (int32 i = 0; i < threadNum_; ++i) {
            char name[32];
            snprintf(name, 32, "%s%d", "SocketDispatherWorkEventLoop-", i);
            workerThreads_[i].setName(name);
            //workerThreads_[i].setOwner(this);
        }
    }

    if (!mainEventLoop_.init()) {
        if (workerThreads_) {
            delete []workerThreads_;
            workerThreads_ = NULL;
        }
        return false;
    }
    if (!mainEventLoop_.start()) {
        if (workerThreads_) {
            delete []workerThreads_;
            workerThreads_ = NULL;
        }
        return false;
    }

    bool ret = false;
    for (int32 i = 0; i < threadNum_; ++i) {
        if (!workerThreads_[i].start()) {
            ret = true;
            break;
        }
    }

    if (ret) {
        for (int32 i = 0; i < threadNum_; ++i) {
            workerThreads_[i].stop();
        }
        if (workerThreads_) {
            delete []workerThreads_;
            workerThreads_ = NULL;
        }
        mainEventLoop_.stop();
        mainEventLoop_.wakeup();
        return false;
    }
    return true;
}

bool XDSocketDispather::stop()
{
    mainEventLoop_.stop();
    mainEventLoop_.wakeup();
    if (workerThreads_) {
        for (int32 i = 0; i < threadNum_; ++i) {
            workerThreads_[i].stop();
        }
        delete []workerThreads_;
        workerThreads_ = NULL;
    }
    return true;
}

bool XDSocketDispather::acceptSocket(XDSockFD socket, void *ud)
{
    return mainEventLoop_.add(socket, ud);
}

int32 XDSocketDispather::addSocket(XDSockFD socket, void *ud)
{
    if (threadNum_ > 0) {
        int32 index = workerIndex_ % threadNum_;
        if (workerThreads_[index].add(socket, ud)) {
            ++workerIndex_;
            return index;
        }
        return -2;
    } else {
        if (mainEventLoop_.add(socket, ud)) {
            return -1;
        }
        return -2;
    }
}

bool XDSocketDispather::delSocket(XDSockFD socket, int32 index)
{
    bool ret = false;
    if (-1 == index) {
        // main loop
        ret = mainEventLoop_.del(socket);
    } else {
        ret = workerThreads_[index].del(socket);
    }
    return ret;
}

bool XDSocketDispather::writeSocket(XDSockFD socket, int32 index, void *ud, bool enable)
{
    bool ret = false;
    if (-1 == index) {
        // main loop
        ret = mainEventLoop_.write(socket, ud, enable);
    } else {
        ret = workerThreads_[index].write(socket, ud, enable);
    }
    return ret;
}
