#ifndef XD_SOCKET_HANDLER_H
#define XD_SOCKET_HANDLER_H

#include "../base/base_object.h"
#include "socket_connection.h"

class XDMessage;

class XDTcpServerSocketEventHandler : public XDBaseObject
{
public:
    XDTcpServerSocketEventHandler() { }
    virtual ~XDTcpServerSocketEventHandler() { }

    virtual void onAccept(XDSocketConnectionPtr socket) = 0;
    virtual void onDisconnect(XDSocketConnectionPtr socket) = 0;
    virtual void onMessage(XDSocketConnectionPtr socket, XDMessage& message) = 0;
};

#endif // end xd_socket_handler_h
