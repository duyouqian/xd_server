#ifndef XD_SOCKET_SERVER_H
#define XD_SOCKET_SERVER_H

#include "../base/base_object.h"
#include "socket_acceptor.h"

namespace XDSocketPoll {
struct XDEvent;
}

class XDMessage;
class XDNetService;

class XDSocketServer : public XDBaseObject
{
    static XDHandle sHandle_;
protected:
    enum XDSocketServerState
    {
        ST_INIT,
        ST_INITED,
        ST_STARTED,
        ST_CLOSED
    };
public:
    explicit XDSocketServer(XDNetService &netService);
    virtual ~XDSocketServer();

    virtual bool init(int32 port) = 0;
    virtual bool start();
    virtual bool stop();

    // for event loop thread
    virtual bool accept() = 0;
    virtual void connMessageCallBack(XDHandle handle, XDMessage &message) = 0;
    virtual void connDisconnectCallBack(XDHandle handle) = 0;
    virtual void connSendMessageCallBack(XDHandle handle, bool enable) = 0;

    XDHandle handle() const { return handle_; }

protected:
    XDSocketAccept serverSocketFD_;
    XDNetService &netService_;
    XDSocketServerState state_;
    bool isStop_;
    XDHandle handle_;
};

#endif // end xd_socket_server_h
