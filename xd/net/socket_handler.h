#ifndef XD_SOCKET_HANDLER_H
#define XD_SOCKET_HANDLER_H

#include "../base/base_object.h"

class XDMessage;

class XDTcpServerSocketEventHandler : public XDBaseObject
{
public:
    XDTcpServerSocketEventHandler() { }
    virtual ~XDTcpServerSocketEventHandler() { }

    virtual void onAccept(XDSocketPtr socket) = 0;
    virtual void onDisconnect(XDSocketPtr socket) = 0;
    virtual void onMessage(XDSocketPtr socket, XDMessage &message) = 0;
};

class XDTcpClientSocketEventHandler : public XDBaseObject
{
public:
    XDTcpClientSocketEventHandler() { }
    virtual ~XDTcpClientSocketEventHandler() { }
    
    virtual void onConnect(XDSocketPtr socket) = 0;
    virtual void onReConnect(XDSocketPtr socket) = 0;
    virtual void onDisconnect(XDSocketPtr socket) = 0;
    virtual void onMessage(XDSocketPtr socket, XDMessage &message) = 0;
};

class XDInputMessageHandler : public XDBaseObject
{
public:
    virtual ~XDInputMessageHandler() {}
    virtual void handleMessage(XDSocketPtr socket, XDMessage &message) = 0;
};

class XDReplyMessageHandler : public XDBaseObject
{
public:
    virtual ~XDReplyMessageHandler() { }
    virtual void handleMessage(XDSocketPtr socket, XDMessage &message) = 0;
    virtual void handleException(int32 error) = 0;
};

#endif // end xd_socket_handler_h
