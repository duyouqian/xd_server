#include "socket_acceptor.h"
#include "tcp_server.h"
#include "../base/socket_util.h"
#include "../base/log.h"
#include <stdio.h>

XDSocketAccept::XDSocketAccept(XDSocketServer *socketServer)
              : XDBaseSocket::XDBaseSocket(socketServer, XDSocketType::SOCKETTYPE_ACCEPTOR)
{

}

XDSocketAccept::XDSocketAccept(XDSocketServer *socketServer, const char *ip, int32 port)
               : XDBaseSocket::XDBaseSocket(socketServer, XDSocketType::SOCKETTYPE_ACCEPTOR)
{
    snprintf(addr_.ip, sizeof(addr_.ip), "%s", ip);
    addr_.port = port;
}

XDSocketAccept::XDSocketAccept(XDSocketServer *socketServer, int32 port)
              : XDBaseSocket::XDBaseSocket(socketServer, XDSocketType::SOCKETTYPE_ACCEPTOR)
{
    snprintf(addr_.ip, sizeof(addr_.ip), "%s", "0.0.0.0");
    addr_.port = port;
}

XDSocketAccept::~XDSocketAccept()
{

}

bool XDSocketAccept::init(const char *ip, int32 port)
{
    if (state_ != ST_INIT && state_ != ST_CLOSED) {
        return false;
    }
    snprintf(addr_.ip, sizeof(addr_.ip), "%s", ip);
    addr_.port = port;
    return true;
}

bool XDSocketAccept::init(int32 port)
{
    if (state_ != ST_INIT && state_ != ST_CLOSED) {
        return false;
    }
    snprintf(addr_.ip, sizeof(addr_.ip), "%s", "0.0.0.0");
    addr_.port = port;
    return true;
}

bool XDSocketAccept::create()
{
    if (state_ != ST_INIT && state_ != ST_CLOSED) {
        return false;
    }
    fd_ = XDSocketUtil::createServerSocket(addr_.ip, addr_.port, XD_SOCKETUTIL_BACKLOG);
    if (XDSocketUtil::isInvalid(fd_)) {
        return false;
    }
    state_ = ST_CONNECTED;
    return true;
}

XDSockFD XDSocketAccept::accept(XDSocketUtil::XDSockAddr *addr)
{
    if (state_ != ST_CONNECTED) {
        return -1;
    }
    XDSockFD connFD = XDSocketUtil::accept(fd_, addr);
    return connFD;
}

void XDSocketAccept::accept()
{
    socketServer_->accept();
}
