#include "net_service.h"
#include "../net/socket_handler.h"

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
    socket->onClose();
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
    return true;
}

void XDNetService::afterStart()
{

}

void XDNetService::stop()
{

}

void XDNetService::frontedListen(int32 port)
{
    static XDNetServerSocketEventHandler defaultFrontedTcpHandler(*this);
    frontedHandler_ = &defaultFrontedTcpHandler;
    frontedTcpServer_.setHandler(frontedHandler_);
    frontedTcpServer_.init(port);
}
