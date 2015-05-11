#ifndef XD_SOCKET_DISPATHER_H
#define XD_SOCKET_DISPATHER_H

#include "../base/base_object.h"
#include "event_loop.h"

#include <map>

class XDSocketWorkThread;

// socket 调度器

class XDSocketDispather : public XDBaseObject
{
public:
    XDSocketDispather(int32 threadNum = 1);
    ~XDSocketDispather();

    bool start();
    bool stop();

    void setThreadNum(int32 num) { threadNum_ = num > 1 ? num - 1 : 0; }

    // return -2: failure, -1 : main loop , other work thread index
    // main thread for event loop
    bool acceptSocket(XDSockFD socket, void *ud);
    // work thread for event loop
    int32 addSocket(XDSockFD socket, void *ud);
    bool delSocket(XDSockFD socket, int32 index);
    bool writeSocket(XDSockFD socket, int32 index, void *ud, bool enable);

    FROCEINLINE bool isInvalidIndex(int32 index)
    {
        if (-2 == index) {
            return true;
        }
        if (-1 == index || (index >= 0 && index < threadNum_)) {
            return false;
        }
    }

private:
    XDEventLoop mainEventLoop_;
    XDMutex mutex_;
    int32 threadNum_;
    XDSocketWorkThread *workerThreads_;
    int32 workerIndex_;
};

#endif // end xd_socket_dispather_h
