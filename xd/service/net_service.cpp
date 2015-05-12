#include "net_service.h"
#include "../net/socket_handler.h"
#include "../net/socket_connecter.h"
#include "../base/log.h"

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
    service_.processMessage(socket, message);
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
    if (!frontedHandler_) {
        static XDNetServerSocketEventHandler defaultFrontedHandler(*this);
        frontedHandler_ = &defaultFrontedHandler;
    }
    frontedTcpServer_ = listen(5000, frontedHandler_);
    check(frontedTcpServer_);
    frontedTcpServer_->start();
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

void XDNetService::processMessage(XDSocketConnectionPtr socket, XDMessage &message)
{
    if (message.header().flags & XDMESSAGE_FLAG_HASRELPID &&
        message.header().cmdID == XDRESPONSE_MESSAGE_ID) {
        // reply
        std::map<XDReplyID, XDReplyRecord>::iterator it =
                replyRecordMap_.find(message.replyID());
        if (it != replyRecordMap_.end()) {
            XDReplyRecord &record = it->second;
            // timer
            //record.handler->handleMessage(socket, message);
            replyRecordMap_.erase(it);
        } else {
            XD_LOG_mwarn("[NetService] 消息ID:%d replyID:%d 没有执行体", message.header().cmdID, message.replyID());
        }
    } else {
        // request
        std::map<XDCmdID, XDInputMessageHandler*>::iterator it =
                messageHandlerMap_.find(message.header().cmdID);
        if (it != messageHandlerMap_.end()) {
            XDInputMessageHandler *handler = it->second;
            //handler->handleMessage(socket, message);
        } else {
            XD_LOG_mwarn("[NetService] 消息ID:%d 没有执行体", message.header().cmdID);
        }
    }
}

XDReplyID XDNetService::getNextReplyID()
{
    const XDReplyID MAX_REPLY_ID = 0x7FFFFFFFFFFFFFFF;
    if (++nextReplyID_ > MAX_REPLY_ID) {
        nextReplyID_ = 1;
    }
    return nextReplyID_;
}

void XDNetService::registerMessageHandler(XDCmdID cmdID, XDInputMessageHandler *handler)
{
    if (messageHandlerMap_.find(cmdID) != messageHandlerMap_.end()) {
        return;
    }
    messageHandlerMap_[cmdID] = handler;
}

void XDNetService::registerReplyRecord(XDReplyID replyID, XDReplyMessageHandler *handler)
{
    if (replyRecordMap_.find(replyID) != replyRecordMap_.end()) {
        return;
    }
    XDReplyRecord res;
    res.handler = handler;
    replyRecordMap_[replyID] = res;
}

XDReplyRecord XDNetService::removeReplyRecord(XDReplyID replyID)
{
    XDReplyRecord res;
    std::map<XDReplyID, XDReplyRecord>::iterator it = replyRecordMap_.find(replyID);
    if (it != replyRecordMap_.end()) {
        res = it->second;
        replyRecordMap_.erase(it);
    }
    return res;
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
