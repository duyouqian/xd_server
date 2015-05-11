#ifndef XD_TCP_SERVER_H
#define XD_TCP_SERVER_H

#include "socket_server.h"
#include "../base/mutex.h"
#include <map>

class XDTcpServerSocketEventHandler;

class XDTcpServer : public XDSocketServer
{
public:
    explicit XDTcpServer(XDNetService &netService);
    ~XDTcpServer();

    void setHandler(XDTcpServerSocketEventHandler *handler) { handler_ = handler; }

    bool init(int32 port);
    //virtual bool start();
    virtual bool stop();

    // for event loop thread
    bool accept();
    void connMessageCallBack(XDHandle handle, XDMessage &message);
    void connDisconnectCallBack(XDHandle handle);
    void connSendMessageCallBack(XDHandle handle, bool enable);

protected:
    void addSocket(XDSocketConnectionPtr socket);
    void delSocketByHandle(XDHandle handle);
    XDSocketConnectionPtr findSocketByHandle(XDHandle handle);

protected:
    XDTcpServerSocketEventHandler *handler_;
    XDMutex mutex_;
    std::map<XDHandle, XDSocketConnectionPtr> conns_;
};

#endif // end xd_tcp_server_h
