#ifndef XD_SOCKET_CONNECTER_H
#define XD_SOCKET_CONNECTER_H

#include "socket_connection.h"

class XDNetService;

class XDTcpClientSocketEventHandler;

class XDSocketConnecter : public XDSocketConnection
{
public:
    XDSocketConnecter();
    ~XDSocketConnecter();
    
    bool connect(const char *host, int32 port, XDTcpClientSocketEventHandler *handler, bool isReconnect = true, int32 maxReconnectAttempts = -1);
    void setEventHandler(XDTcpClientSocketEventHandler *handler) { handler_ = handler; }
    
protected:
    virtual void connMessageCallBack();
    virtual void connDisconnectCallBack();
    virtual void connSendMessageCallBack(bool enable);
    
protected:
    bool startConnect();
    
protected:
    XDTcpClientSocketEventHandler *handler_;
    bool isReconnect_;
    int32 maxReconnectAttempts_;
    int32 reconnectAttempts_;
};

#endif // end xd_socket_connecter_h