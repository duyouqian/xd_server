#ifndef XD_TCP_SERVER_DISPATCHER_H
#define XD_TCP_SERVER_DISPATCHER_H

#include "socket_server_dispatcher.h"
#include "socket_connection.h"
#include "../base/mutex.h"

#include <map>

class XDTcpServer;

class XDTcpServerDispatcher : public XDSocketServerDispatcher
{
public:
    XDTcpServerDispatcher(XDTcpServer *server);
    XDTcpServerDispatcher(XDTcpServer *server, int32 threadNum_);
    ~XDTcpServerDispatcher();

    virtual bool init();

    // for socket-server thread --singleton thread
    virtual bool accept();
    virtual bool accept(XDSocketConnectionPtr newConn);
    // for socket-worker thread --multi thread 需要加锁
    //virtual bool handleReadMessage(XDSocketPoll::XDEvent *event);
    virtual bool handleSendMessage(XDSocketConnectionPtr newConn, bool enable);
    virtual bool disconnect(XDHandle handle);
    virtual bool closeSocket(XDHandle handle);

    XDSocketConnectionPtr findConnectByHandle(XDHandle handle);

private:
    XDTcpServer* server_;
    std::map<XDHandle, XDSocketConnectionPtr> conns_;
    XDMutex mutex_;
};

#endif // end xd_tcp_server_dispatcher_h
