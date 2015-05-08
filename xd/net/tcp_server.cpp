#include "tcp_server.h"
#include "socket_connection.h"
#include "socket_poll.h"
#include "message.h"
#include "socket_handler.h"
#include "../base/log.h"
#include <functional>

#define XD_IOEVENTTYPE_ACCEPT 0
#define XD_IOEVENTTYPE_MESSAGE 1
#define XD_IOEVENTTYPE_CLOSE 2

class XDTcpIOEventTask : public XDITask
{
public:
    XDTcpIOEventTask(int32 type) : message(), eventType(type) { }
    ~XDTcpIOEventTask()
    {

    }

    virtual void operator ()()
    {
        if (XD_IOEVENTTYPE_ACCEPT == eventType) {
            handler->onAccept(conn);
        } else if (XD_IOEVENTTYPE_CLOSE == eventType) {
            handler->onDisconnect(conn);
        } else if (XD_IOEVENTTYPE_MESSAGE == eventType) {
            handler->onMessage(conn, message);
        }
    }

    XDSocketConnectionPtr conn;
    XDMessage message;
    XDTcpServerSocketEventHandler *handler;
    int32 eventType; // 0: create 1: message 2: disconnection
};

XDTcpServer::XDTcpServer()
           : XDSocketServer::XDSocketServer()
           , dispatcher_(this, 5)
{

}

XDTcpServer::~XDTcpServer()
{
}

bool XDTcpServer::init(int32 port)
{
    if (state_ != ST_INIT) {
        return false;
    }
    serverSocketFD_.init(port);
    dispatcher_.init();
    state_ = ST_INITED;
    return true;
}

bool XDTcpServer::start()
{
    bool ret = XDSocketServer::start();
    if (ret) {
        dispatcher_.run();
    }
    return ret;
}

bool XDTcpServer::accept()
{
    XD_LOG_mdebug("[TcpServer] serverName:%s 有新连接", "TcpServer");
    XDSocketConnectionPtr conn(new XDSocketConnection(*this));
    XDSockFD connFD = serverSocketFD_.accept(conn->addressPtr());
    if (-1 == connFD) {
        XD_LOG_mdebug("[TcpServer] serverName:%s accept 失败", "TcpServer");
        return false;
    }
    conn->setFD(connFD);
    if (!conn->create()) {
        XD_LOG_mdebug("[TcpServer] serverName:%s connection create 失败 [handle:%d fd:%d]", "TcpServer", conn->handle(), connFD);
        conn.reset();
        return false;
    }
    //conn->setMessageCallBack(std::bind(&XDTcpServer::connMessageCallBack, this, std::placeholders::_1, std::placeholders::_2));
    //conn->setDisconnectCallBack(std::bind(&XDTcpServer::connDisconnectCallBack, this, std::placeholders::_1));
    dispatcher_.accept(conn);
    XDTcpIOEventTask *task = new XDTcpIOEventTask(XD_IOEVENTTYPE_ACCEPT);
    task->conn = conn;
    task->handler = handler_;
    check(task);
    addTask(task);
//    if (handler_) {
//        runInLoop(std::bind(&XDTcpServerSocketEventHandler::onAccept, handler_, conn));
//    }
    XD_LOG_mdebug("[TcpServer] serverName:%s 接收新连接成功 [handle:%d fd:%d ip:%s:%d]", "TcpServer", conn->handle(), connFD, conn->address().ip, conn->address().port);
    return true;
}

void XDTcpServer::connMessageCallBack(XDHandle handle, XDMessage &message)
{
    XDSocketConnectionPtr conn = dispatcher_.findConnectByHandle(handle);
    if (conn && handler_) {
        XDTcpIOEventTask *task = new XDTcpIOEventTask(XD_IOEVENTTYPE_MESSAGE);
        task->conn = conn;
        task->handler = handler_;
        task->message = message;
        addTask(task);
        //runInLoop(std::bind(&XDTcpServerSocketEventHandler::onMessage, handler_, conn, message));
    }
}

void XDTcpServer::connDisconnectCallBack(XDHandle handle)
{
    XDSocketConnectionPtr conn = dispatcher_.findConnectByHandle(handle);
    if (conn && handler_) {
        XDTcpIOEventTask *task = new XDTcpIOEventTask(XD_IOEVENTTYPE_CLOSE);
        task->conn = conn;
        task->handler = handler_;
        addTask(task);
        //runInLoop(std::bind(&XDTcpServerSocketEventHandler::onDisconnect, handler_, conn));
    }
    dispatcher_.closeSocket(handle);
}
