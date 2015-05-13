#ifndef XD_NET_SERVICE_H
#define XD_NET_SERVICE_H

#include "service.h"
#include "../net/socket_dispatcher.h"
#include "../net/tcp_server.h"

#include <map>

#define XD_NET_SERVICE_NAME "NetService"

class XDApp;
class XDTcpServerSocketEventHandler;
class XDTcpClientSocketEventHandler;
class XDInputMessageHandler;
class XDReplyMessageHandler;

// 消息回复记录
struct XDReplyRecord
{
    XDReplyMessageHandler* handler;
};

class XDNetService : public XDBaseService
{
public:
    XDNetService(XDApp &app);
    ~XDNetService();

    virtual bool start();
    virtual void afterStart();
    virtual void stop();

    // 前端服务器监听
    //void frontedListen(int32 port);
    // 后端RPC服务器监听
    //void backendRpcListen(int32 port);
    void setFrontedTCPServerEventHandler(XDTcpServerSocketEventHandler* handler);
    void setBackendTCPServerEventHandler(XDTcpServerSocketEventHandler* handler);
    
    // 连接(rpc服务器)
    XDSocketConnecterPtr connect(const char *host, int32 port, XDTcpClientSocketEventHandler *handler, bool isReconnect = true, int32 maxReconnectAttempts = -1);
    // 监听
    XDTcpServerPtr listen(int32 port, XDTcpServerSocketEventHandler *handler);
    // socket 调度器
    XDSocketDispather& socketDispather() { return socketDispather_; }

    // process message
    void processMessage(XDSocketPtr socket, XDMessage &message);
    uint64 getNextReplyID();
    void registerMessageHandler(XDCmdID cmdID, XDInputMessageHandler *handler);
    void registerReplyRecord(XDReplyID replyID, XDReplyMessageHandler *handler);
    // 移除回复记录
    XDReplyRecord removeReplyRecord(XDReplyID id);
    
protected:
    void stopTcpServer();
    void stopClientSocket();

protected:
    XDSocketDispather socketDispather_;
    XDTcpServerPtr frontedTcpServer_;
    XDTcpServerSocketEventHandler *frontedHandler_;
    XDTcpServerPtr backendTcpServer_;
    XDTcpServerSocketEventHandler *backendHandler_;
    
    std::map<XDHandle, XDTcpServerPtr> tcpServerMap_;
    std::map<XDHandle, XDSocketConnecterPtr> clientSocketMap_;


    XDReplyID nextReplyID_;
    // 消息处理 <cmdID, handler>
    std::map<XDCmdID, XDInputMessageHandler*> messageHandlerMap_;
    // 回复处理 <ReplyID, handler>
    std::map<XDReplyID, XDReplyRecord> replyRecordMap_;
};

#endif // end xd_net_service_h
