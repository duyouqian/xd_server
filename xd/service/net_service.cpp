#include "net_service.h"
#include "../net/socket_handler.h"

class XDNetServerSocketEventHandler : public XDTcpServerSocketEventHandler
{
public:
    XDNetServerSocketEventHandler(XDNetService &service);
    ~XDNetServerSocketEventHandler();
    virtual void onAccept(XDSocketConnection::ConnPtr socket);
    virtual void onDisconnect(XDSocketConnection::ConnPtr socket);
    virtual void onMessage(XDSocketConnection::ConnPtr socket, XDMessage& message);

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

void XDNetServerSocketEventHandler::onAccept(XDSocketConnection::ConnPtr socket)
{

}

void XDNetServerSocketEventHandler::onDisconnect(XDSocketConnection::ConnPtr socket)
{

}

void XDNetServerSocketEventHandler::onMessage(XDSocketConnection::ConnPtr socket, XDMessage& message)
{

}

XDNetService::XDNetService(XDApp &app)
            : XDBaseService(app, XD_NET_SERVICE_NAME)
{

}

XDNetService::~XDNetService()
{

}

bool XDNetService::start()
{
    if (frontedTcpServer_.start()) {
        frontedTcpServer_.poll();
    }
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
