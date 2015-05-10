#ifndef XD_NET_SERVICE_H
#define XD_NET_SERVICE_H

#include "service.h"
#include "../net/tcp_server.h"

#define XD_NET_SERVICE_NAME "NetService"

class XDApp;
class XDTcpServerSocketEventHandler;
class XDTcpClientSocketEventHandler;

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
    void backendRpcListen(int32 port);
    
    // 连接rpc服务器
    XDSocketConnecterPtr rpcConnect(const char *host, int32 port, XDTcpClientSocketEventHandler *handler, bool isReconnect = true, int32 maxReconnectAttempts = -1);

protected:
    XDTcpServer frontedTcpServer_;
    XDTcpServerSocketEventHandler *frontedHandler_;
};

#endif // end xd_net_service_h
