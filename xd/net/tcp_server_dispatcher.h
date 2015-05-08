#ifndef XD_TCP_SERVER_DISPATCHER_H
#define XD_TCP_SERVER_DISPATCHER_H

#include "socket_server_dispatcher.h"
#include "socket_connection.h"
#include "../base/mutex.h"

#include <map>

class XDTcpServerDispatcher : public XDSocketServerDispatcher
{
public:
    XDTcpServerDispatcher(XDTcpServer *server);
    XDTcpServerDispatcher(XDTcpServer *server, int32 threadNum_);
    ~XDTcpServerDispatcher();

    virtual bool init();

    // for socket-server thread --singleton thread
    virtual bool accept();
    virtual bool accept(XDSocketConnection::ConnPtr newConn);
    // for socket-worker thread --multi thread 需要加锁
    virtual bool handleReadMessage(XDSocketPoll::XDEvent *event);
    virtual bool handleSendMessage(XDSocketPoll::XDEvent *event);
    virtual bool disconnect(XDHandle handle);
    virtual bool closeSocket(XDHandle handle);

    XDSocketConnection::ConnPtr findConnectByHandle(XDHandle handle);

private:
    XDTcpServer* server_;
    std::map<XDHandle, XDSocketConnection::ConnPtr> conns_;
    XDMutex mutex_;
};

#endif // end xd_tcp_server_dispatcher_h
