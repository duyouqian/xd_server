#include "tcp_server.h"
#include "socket_connection.h"
#include "rpc_message.h"
#include "socket_handler.h"
#include "../service/app.h"
#include "../service/net_service.h"
#include "../base/log.h"
#include "../base/interface.h"
#include "../base/guard_mutex.h"
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

XDTcpServer::XDTcpServer(XDNetService &netService)
           : XDSocketServer::XDSocketServer(netService)
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
    state_ = ST_INITED;
    return true;
}

bool XDTcpServer::stop()
{
    XDSocketServer::stop();
    conns_.clear();
}

bool XDTcpServer::accept()
{
    XD_LOG_mdebug("[TcpServer] serverName:%s 有新连接", "TcpServer");
    XDSocketConnectionPtr conn(new XDSocketConnection(this));
    XDSockFD connFD = serverSocketFD_.accept(conn->addressPtr());
    if (-1 == connFD) {
        XD_LOG_mdebug("[TcpServer] serverName:%s accept 失败", "TcpServer");
        return false;
    }
    // 设置为非堵塞IO
    XDSocketUtil::setSocketNonblock(connFD, true);
    conn->setFD(connFD);
    if (!conn->create()) {
        XD_LOG_mdebug("[TcpServer] serverName:%s connection create 失败 [handle:%d fd:%d]", "TcpServer", conn->handle(), connFD);
        conn.reset();
        return false;
    }
    //dispatcher_.accept(conn);
    int32 index = netService_.socketDispather().addSocket(conn->fd(), conn.get());
    if (netService_.socketDispather().isInvalidIndex(index)) {
        XD_LOG_mdebug("[TcpServer] serverName:%s connection add poll 失败 [handle:%d fd:%d]", "TcpServer", conn->handle(), connFD);
        conn.reset();
        return false;
    }
    conn->setWorkThreadIndex(index);
    addSocket(conn);
    XDTcpIOEventTask *task = new XDTcpIOEventTask(XD_IOEVENTTYPE_ACCEPT);
    task->conn = conn;
    task->handler = handler_;
    check(task);
    netService_.app().post(task);
    //addTask(task);
    XD_LOG_mdebug("[TcpServer] serverName:%s 接收新连接成功 [handle:%d fd:%d ip:%s:%d]", "TcpServer", conn->handle(), connFD, conn->address().ip, conn->address().port);
    return true;
}

void XDTcpServer::connMessageCallBack(XDHandle handle, XDMessage &message)
{
    XDSocketConnectionPtr conn = findSocketByHandle(handle);
    if (conn && handler_) {
        XDTcpIOEventTask *task = new XDTcpIOEventTask(XD_IOEVENTTYPE_MESSAGE);
        task->conn = conn;
        task->handler = handler_;
        task->message = message;
        netService_.app().post(task);
    }
}

void XDTcpServer::connDisconnectCallBack(XDHandle handle)
{
    XDSocketConnectionPtr conn = findSocketByHandle(handle);
    if (conn) {
        if (handler_) {
            XDTcpIOEventTask *task = new XDTcpIOEventTask(XD_IOEVENTTYPE_CLOSE);
            task->conn = conn;
            task->handler = handler_;
            netService_.app().post(task);
        }
        netService_.socketDispather().delSocket(conn->fd(), conn->workThreadIndex());
        XD_LOG_mdebug("[TcpServer] handle:%d fd:%d 断开", handle, conn->fd());
        conn->close();
        delSocketByHandle(conn->handle());
    }
}

void XDTcpServer::connSendMessageCallBack(XDHandle handle, bool enable)
{
    XDSocketConnectionPtr conn = findSocketByHandle(handle);
    if (conn) {
        netService_.socketDispather().writeSocket(conn->fd(), conn->workThreadIndex(), conn.get(), enable);
    }
}

void XDTcpServer::addSocket(XDSocketConnectionPtr socket)
{
    XDGuardMutex lock(&mutex_);
    std::map<XDHandle, XDSocketConnectionPtr>::const_iterator it = conns_.find(socket->handle());
    if (conns_.end() == it) {
        conns_[socket->handle()] = socket;
    }
}

void XDTcpServer::delSocketByHandle(XDHandle handle)
{
    XDGuardMutex lock(&mutex_);
    std::map<XDHandle, XDSocketConnectionPtr>::iterator it = conns_.find(handle);
    if (conns_.end() != it) {
        conns_.erase(it);
    }
}

XDSocketConnectionPtr XDTcpServer::findSocketByHandle(XDHandle handle)
{
    XDGuardMutex lock(&mutex_);
    std::map<XDHandle, XDSocketConnectionPtr>::const_iterator it = conns_.find(handle);
    if (conns_.end() != it) {
        return it->second;
    }
    return NULL;
}
