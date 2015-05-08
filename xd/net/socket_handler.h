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

    virtual void onAccept(XDSocketConnection::ConnPtr socket) = 0;
    virtual void onDisconnect(XDSocketConnection::ConnPtr socket) = 0;
    virtual void onMessage(XDSocketConnection::ConnPtr socket, XDMessage& message) = 0;
};

#endif // end xd_socket_handler_h
