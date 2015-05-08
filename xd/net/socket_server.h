#ifndef XD_SOCKET_SERVER_H
#define XD_SOCKET_SERVER_H

#include "../base/base_object.h"
#include "../base/mutex.h"
#include "socket_acceptor.h"
#include <functional>
#include <vector>

namespace XDSocketPoll {
struct XDEvent;
}

class XDITask
{
public:
    virtual void operator ()() {}
    virtual ~XDITask()
    {

    }
};

class XDSocketServer : public XDBaseObject
{
    typedef std::function<void()> XDFunction;
    static XDHandle sHandle_;
protected:
    enum XDSocketServerState
    {
        ST_INIT,
        ST_INITED,
        ST_STARTED,
        ST_CLOSED
    };
public:
    XDSocketServer();
    virtual ~XDSocketServer();

    virtual bool init(int32 port) = 0;
    virtual bool start();
    virtual bool stop();

    int32 poll();
    int32 poll(int32 timeout);
    void wakeup();

    virtual bool accept() = 0;
//    virtual bool handleReadMessage(XDSocketPoll::XDEvent *event) = 0;
//    virtual bool handleSendMessage(XDSocketPoll::XDEvent *event) = 0;
//    virtual bool disconnect(XDHandle handle) = 0;
//    virtual bool closeSocket(XDHandle handle) = 0;
//    virtual bool acceptNewConnection(XDSockFD newConn) = 0;

    // 来自相同线程中运行
    void runInLoop(const XDFunction &cb);
    // 来自其他线程
    void queueInLoop(const XDFunction &cb);

    XDHandle handle() const { return handle_; }

protected:
    void doPendingFunctors();
    bool isInLoopThread();

    void addTask(XDITask* task);
    void doPendingTask();

protected:
    XDSocketAccept serverSocketFD_;
    XDPollFD pollFD_;
    XDSocketServerState state_;
    bool isStop_;
    XDSockFD recvFD_; // 管道接收端
    XDSockFD sendFD_; // 管道发送端
    bool callingPendingFunctors_; // 是否要处理等待队列
    std::vector<XDFunction> pendingFunctors_; // 存储来自其他线程的处理器
    bool callingPendingTasks_;
    std::vector<XDITask*> pendingTask_; // 任务队列
    XDMutex mutex_;
    XDTID threadID_;
    XDHandle handle_;
};

#endif // end xd_socket_server_h
