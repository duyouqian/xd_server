#ifndef XD_TCP_SERVER_H
#define XD_TCP_SERVER_H

#include "socket_server.h"
#include "tcp_server_dispatcher.h"

class XDTcpServerSocketEventHandler;
class XDMessage;

class XDTcpServer : public XDSocketServer
{
public:
    XDTcpServer();
    ~XDTcpServer();

    void setHandler(XDTcpServerSocketEventHandler *handler) { handler_ = handler; }

    bool init(int32 port);
    virtual bool start();
    virtual bool accept();

public:
    void connMessageCallBack(XDHandle handle, XDMessage &message);
    void connDisconnectCallBack(XDHandle handle);
    void connSendMessageCallBack(XDHandle handle, bool enable);

private:
    XDTcpServerDispatcher dispatcher_;
    XDTcpServerSocketEventHandler *handler_;
};

#endif // end xd_tcp_server_h
