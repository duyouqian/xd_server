#include "tcp_server_dispatcher.h"
#include "tcp_server.h"
#include "socket_poll.h"
#include "../base/log.h"
#include "../base/guard_mutex.h"

class XDTcpSocketServerWorkThread : public XDSocketServerWorkThread
{
public:
    virtual bool handleReadMessage(XDSocketPoll::XDEvent *event);
    virtual bool handleSendMessage(XDSocketPoll::XDEvent *event);
    virtual bool closeSocket(XDSockFD fd);
};

bool XDTcpSocketServerWorkThread::handleReadMessage(XDSocketPoll::XDEvent *event)
{
    XDTID curTid = XDBaseThread::getCurrentThreadID();
    XDSocketConnection *conn = (XDSocketConnection*)event->ptr;
    conn->read();
    return true;
}

bool XDTcpSocketServerWorkThread::closeSocket(XDSockFD fd)
{
    XDSocketPoll::del(pollFD_, fd);
}

bool XDTcpSocketServerWorkThread::handleSendMessage(XDSocketPoll::XDEvent *event)
{

}

XDTcpServerDispatcher::XDTcpServerDispatcher(XDTcpServer *server)
                     : XDSocketServerDispatcher::XDSocketServerDispatcher()
                     , server_(server)
{
}

XDTcpServerDispatcher::XDTcpServerDispatcher(XDTcpServer *server, int32 threadNum_)
                     : XDSocketServerDispatcher::XDSocketServerDispatcher(threadNum_)
                     , server_(server)
{
}

XDTcpServerDispatcher::~XDTcpServerDispatcher()
{

}

bool XDTcpServerDispatcher::init()
{
    // XDSocketServerWorkThread
    // new (int (*[10])());
    workerThreads_ = new XDTcpSocketServerWorkThread[threadNum_];
    for (int32 i = 0; i < threadNum_; ++i) {
        char name[32];
        snprintf(name, 32, "%s%d", "Socket-worker-thread-", i);
        workerThreads_[i].setName(name);
        workerThreads_[i].setOwner(this);
    }
}

bool XDTcpServerDispatcher::accept()
{
    return true;
}

bool XDTcpServerDispatcher::accept(XDSocketConnection::ConnPtr newConn)
{
    if (!workerThreads_[workerIndex_ % threadNum_].acceptNewConnect(newConn->fd(), newConn.get())) {
        return false;
    }
    newConn->setWorkThreadIndex(workerIndex_);
    ++workerIndex_;
    if (conns_.end() == conns_.find(newConn->handle())) {
        conns_[newConn->handle()] = newConn;
    }
    return true;
}

bool XDTcpServerDispatcher::handleReadMessage(XDSocketPoll::XDEvent *event)
{
    XDTID curTid = XDBaseThread::getCurrentThreadID();
    XD_LOG_mdebug("[TcpServer] tid:%d, myTid:%d", curTid, tid_);
    return true;
}


bool XDTcpServerDispatcher::handleSendMessage(XDSocketPoll::XDEvent *event)
{
    XDSocketConnection *conn = (XDSocketConnection*)event->ptr;
    return true;
}

bool XDTcpServerDispatcher::disconnect(XDHandle handle)
{
    XDGuardMutex lock(&mutex_);
    std::map<XDHandle, XDSocketConnection::ConnPtr>::iterator it = conns_.find(handle);
    if (conns_.end() != it) {
        XDSocketConnection::ConnPtr conn = it->second;
        XD_LOG_mdebug("[TcpServer] handle:%d fd:%d 断开", handle, conn->fd());
        conn->close();
        conns_.erase(it);
    }
    return true;
}

bool XDTcpServerDispatcher::closeSocket(XDHandle handle)
{
    XDSocketConnection::ConnPtr conn;
    {
        XDGuardMutex lock(&mutex_);
        std::map<XDHandle, XDSocketConnection::ConnPtr>::iterator it = conns_.find(handle);
        if (conns_.end() != it) {
            conn = it->second;
        }
    }
    if (conn) {
        workerThreads_[conn->workThreadIndex()].closeSocket(conn->fd());
    }
    disconnect(handle);
    return true;
}

XDSocketConnection::ConnPtr XDTcpServerDispatcher::findConnectByHandle(XDHandle handle)
{
    XDGuardMutex lock(&mutex_);
    std::map<XDHandle, XDSocketConnection::ConnPtr>::const_iterator it = conns_.find(handle);
    if (conns_.end() != it) {
        return it->second;
    }
    return NULL;
}
