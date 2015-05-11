#ifndef XD_EVENT_LOOP_H
#define XD_EVENT_LOOP_H

#include "../base/runnable.h"
#include "../base/thread.h"

#include <string>

namespace XDSocketPoll {
struct XDEvent;
}

class XDEventLoop : public XDRunnable
{
public:
    XDEventLoop();
    XDEventLoop(const char *name);
    ~XDEventLoop();

    bool init();
    bool start();
    bool stop();
    bool run();
    void setName(const char *name);
    //bool poll(int32 millisecond);
    bool add(XDSockFD fd, void *ud);
    bool del(XDSockFD fd);
    bool write(XDSockFD fd, void *ud, bool enable);

    void wakeup();

protected:
    bool onAcceptHandler(XDSocketPoll::XDEvent *event);
    bool onReadHandler(XDSocketPoll::XDEvent *event, XDSocketType type);
    bool onWriteHandler(XDSocketPoll::XDEvent *event, XDSocketType type);

private:
    std::string name_;
    XDPollFD pollFD_;
    bool isStop_;
    XDThread thread_;
    XDSockFD recvFD_; // 管道接收端
    XDSockFD sendFD_; // 管道发送端
};

#endif // end xd_event_loop_h
