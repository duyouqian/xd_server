#ifndef XD_SOCKET_HANDLER_H
#define XD_SOCKET_HANDLER_H

#include "../base/base_object.h"

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

class XDTcpClientSocketEventHandler : public XDBaseObject
{
public:
    XDTcpClientSocketEventHandler() { }
    virtual ~XDTcpClientSocketEventHandler() { }
    
    virtual void onConnect(XDSocketConnecterPtr socket) = 0;
    virtual void onReConnect(XDSocketConnecterPtr socket) = 0;
    virtual void onDisconnect(XDSocketConnecterPtr socket) = 0;
    virtual void onMessage(XDSocketConnecterPtr socket, XDMessage& message) = 0;
};

#endif // end xd_socket_handler_h
