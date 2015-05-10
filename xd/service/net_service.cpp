#include "net_service.h"
#include "../net/socket_handler.h"
#include "../net/socket_connecter.h"

// test
#include "../net/socket_poll.h"

class XDNetServerSocketEventHandler : public XDTcpServerSocketEventHandler
{
public:
    XDNetServerSocketEventHandler(XDNetService &service);
    ~XDNetServerSocketEventHandler();
    virtual void onAccept(XDSocketConnectionPtr socket);
    virtual void onDisconnect(XDSocketConnectionPtr socket);
    virtual void onMessage(XDSocketConnectionPtr socket, XDMessage& message);

private:
    XDNetService &service_;
    XDTID threadID_;
};

XDNetServerSocketEventHandler::XDNetServerSocketEventHandler(XDNetService &service)
                             : service_(service)
{

}

XDNetServerSocketEventHandler::~XDNetServerSocketEventHandler()
{

}

void XDNetServerSocketEventHandler::onAccept(XDSocketConnectionPtr socket)
{
    // 创建 session
    //socket->onClose();
//    int32 i = 1;
//    while (i < 10) {
//        XDMessage msg(i, 0, 0);
//        socket->send(msg);
//        ++i;
//    }
}

void XDNetServerSocketEventHandler::onDisconnect(XDSocketConnectionPtr socket)
{
    // 移除 session
}

void XDNetServerSocketEventHandler::onMessage(XDSocketConnectionPtr socket, XDMessage& message)
{
    // 消息处理
}

XDNetService::XDNetService(XDApp &app)
            : XDBaseService(app, XD_NET_SERVICE_NAME)
{

}

XDNetService::~XDNetService()
{

}

// event poll thread
bool XDNetService::start()
{
    if (frontedTcpServer_.start()) {
        frontedTcpServer_.poll();
    }
    return false;
}

void XDNetService::afterStart()
{
    
}

void XDNetService::stop()
{
    
}

XDSocketConnecterPtr XDNetService::rpcConnect(const char *host, int32 port, XDTcpClientSocketEventHandler *handler, bool isReconnect, int32 maxReconnectAttempts)
{
    XDSocketConnecterPtr conn(new XDSocketConnecter());
    bool ret = conn->connect(host, port, handler, isReconnect, maxReconnectAttempts);
    if (ret) {
        //frontedTcpServer_.
        XDSocketPoll::add(frontedTcpServer_.pollFD(), conn->fd(), conn.get());
    }
    return conn;
}

void XDNetService::frontedListen(int32 port)
{
    static XDNetServerSocketEventHandler defaultFrontedTcpHandler(*this);
    frontedHandler_ = &defaultFrontedTcpHandler;
    frontedTcpServer_.setHandler(frontedHandler_);
    frontedTcpServer_.init(port);
}
