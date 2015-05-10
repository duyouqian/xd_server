#include "socket_connecter.h"
#include "../base/socket_util.h"

XDSocketConnecter::XDSocketConnecter()
                 : XDSocketConnection::XDSocketConnection()
                 , handler_(NULL)
                 , isReconnect_(false)
                 , maxReconnectAttempts_(0)
                 , reconnectAttempts_(0)
{
    
}

XDSocketConnecter::~XDSocketConnecter()
{
    
}

bool XDSocketConnecter::connect(const char *host, int32 port, XDTcpClientSocketEventHandler *handler, bool isReconnect, int32 maxReconnectAttempts)
{
    if (state_ != ST_INIT && state_ != ST_CLOSED) {
        return false;
    }
    snprintf(addr_.ip, sizeof(addr_.ip), "%s", host);
    addr_.port = port;
    handler_ = handler;
    isReconnect_ = isReconnect;
    maxReconnectAttempts_ = maxReconnectAttempts;
    if (!startConnect()) {
        return false;
    }
    state_ = ST_CONNECTING;
    return true;
}

bool XDSocketConnecter::startConnect()
{
    fd_ = XDSocketUtil::connectWithNonblock(addr_.ip, addr_.port);
    if (XDSocketUtil::isInvalid(fd_)) {
        // 连接失败
        state_ = ST_CLOSED;
        return false;
    }
    return true;
}

void XDSocketConnecter::connMessageCallBack()
{
    
}

void XDSocketConnecter::connDisconnectCallBack()
{
    
}

void XDSocketConnecter::connSendMessageCallBack(bool enable)
{
    
}