#include "net_service.h"
#include "../net/socket_handler.h"
#include "../net/socket_connecter.h"

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
            , socketDispather_(1)
            , frontedHandler_(NULL)
            , backendHandler_(NULL)
{

}

XDNetService::~XDNetService()
{

}

// event poll thread
bool XDNetService::start()
{
    if (!XDBaseService::start()) {
        return false;
    }
    if (!socketDispather_.start()) {
        return false;
    }
    return true;
}

void XDNetService::afterStart()
{
    
}

void XDNetService::stop()
{
    if (ST_STOPPED == state_) {
        return ;
    }
    
    // stop client socket
    stopClientSocket();
    // stop tcp server (fronted and backend)
    stopTcpServer();
    
    XDBaseService::stop();
}

XDSocketConnecterPtr XDNetService::connect(const char *host, int32 port, XDTcpClientSocketEventHandler *handler, bool isReconnect, int32 maxReconnectAttempts)
{
    XDSocketConnecterPtr conn(new XDSocketConnecter());
    bool ret = conn->connect(host, port, handler, isReconnect, maxReconnectAttempts);
    if (ret) {
        int32 index = socketDispather_.addSocket(conn->fd(), conn.get());
        if (!socketDispather_.isInvalidIndex(index)) {
            clientSocketMap_.insert(std::make_pair(conn->handle(), conn));
        }
    }
    return conn;
}

XDTcpServerPtr XDNetService::listen(int32 port, XDTcpServerSocketEventHandler *handler)
{
    XDTcpServerPtr server(new XDTcpServer(*this));
    server->setHandler(handler);
    server->init(port);
    tcpServerMap_.insert(std::make_pair(server->handle(), server));
    return server;
}

void XDNetService::setFrontedTCPServerEventHandler(XDTcpServerSocketEventHandler* handler)
{
    if (frontedTcpServer_) {
        frontedTcpServer_->setHandler(handler);
    }
    frontedHandler_ = handler;
}

void XDNetService::setBackendTCPServerEventHandler(XDTcpServerSocketEventHandler* handler)
{
    if (backendTcpServer_) {
        backendTcpServer_->setHandler(handler);
    }
    backendHandler_ = handler;
}

void XDNetService::stopTcpServer()
{
    for (std::map<XDHandle, XDTcpServerPtr>::iterator it = tcpServerMap_.begin(); it != tcpServerMap_.end(); ++it) {
        it->second->stop();
    }
}

void XDNetService::stopClientSocket()
{
    for (std::map<XDHandle, XDSocketConnecterPtr>::iterator it = clientSocketMap_.begin(); it != clientSocketMap_.end(); ++it) {
        it->second->close();
    }
}

//void XDNetService::frontedListen(int32 port)
//{
//    static XDNetServerSocketEventHandler defaultFrontedTcpHandler(*this);
//    frontedHandler_ = &defaultFrontedTcpHandler;
//    frontedTcpServer_.setHandler(frontedHandler_);
//    frontedTcpServer_.init(port);
//}
