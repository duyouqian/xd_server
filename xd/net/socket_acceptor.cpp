#include "socket_acceptor.h"
#include "../base/log.h"
#include <stdio.h>

XDSocketAccept::XDSocketAccept()
              : XDBaseSocket::XDBaseSocket()
{

}

XDSocketAccept::XDSocketAccept(const char *ip, int32 port)
               : XDBaseSocket::XDBaseSocket()
{
    snprintf(addr_.ip, sizeof(addr_.ip), "%s", ip);
    addr_.port = port;
}

XDSocketAccept::XDSocketAccept(int32 port)
              : XDBaseSocket::XDBaseSocket()
{
    snprintf(addr_.ip, sizeof(addr_.ip), "%s", "0.0.0.0");
    addr_.port = port;
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
