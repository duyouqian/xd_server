#ifndef XD_NET_SERVICE_H
#define XD_NET_SERVICE_H

#include "service.h"
#include "../net/tcp_server.h"

#define XD_NET_SERVICE_NAME "NetService"

class XDApp;
class XDTcpServerSocketEventHandler;

class XDNetService : public XDBaseService
{
public:
    XDNetService(XDApp &app);
    ~XDNetService();

    virtual bool start();
    virtual void afterStart();
    virtual void stop();

    // 前端服务器监听
    void frontedListen(int32 port);
    // 后端RPC服务器监听
    void backendListen(int32 port);

protected:
    XDTcpServer frontedTcpServer_;
    XDTcpServerSocketEventHandler *frontedHandler_;
};

#endif // end xd_net_service_h
