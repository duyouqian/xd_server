#ifndef XD_SOCKET_SERVER_DISPATCHER_H
#define XD_SOCKET_SERVER_DISPATCHER_H

#include "../base/runnable.h"
#include "../base/types.h"
#include "../base/thread.h"
#include "../base/event.h"

// socket server 调度器

namespace XDSocketPoll {
struct XDEvent;
}
class XDSocketServerDispatcher;

class XDSocketServerWorkThread : public XDRunnable
{
public:
    XDSocketServerWorkThread();
    ~XDSocketServerWorkThread();
    void setOwner(XDSocketServerDispatcher *owner);
    void setName(const char *name);
    bool start();
    bool run();
    bool acceptNewConnect(XDSockFD fd, void *ud);

    virtual bool handleReadMessage(XDSocketPoll::XDEvent *event);
    virtual bool handleSendMessage(XDSocketPoll::XDEvent *event);
    virtual bool closeSocket(XDSockFD fd);

protected:
    XDThread thread_;
    XDPollFD pollFD_;
    bool isStop_;
    XDSocketServerDispatcher *owner_;
    XDSyncEvent setupEvent_;
};

class XDSocketServerDispatcher : public XDRunnable
{
public:
    XDSocketServerDispatcher();
    XDSocketServerDispatcher(int32 threadNum);
    virtual ~XDSocketServerDispatcher();

    virtual bool run();
    virtual bool init();

    // for socket-server thread --singleton thread
    virtual bool accept() = 0;
    // for socket-worker thread --multi thread 需要加锁
    virtual bool handleReadMessage(XDSocketPoll::XDEvent *event) = 0;
    virtual bool handleSendMessage(XDSocketPoll::XDEvent *event) = 0;
    virtual bool disconnect(XDHandle handle) = 0;
    virtual bool closeSocket(XDHandle handle) = 0;

protected:
    int32 threadNum_;
    XDSocketServerWorkThread *workerThreads_;
    int32 workerIndex_;
    XDTID tid_;
};

#endif // end xd_socket_server_dispatcher_h
