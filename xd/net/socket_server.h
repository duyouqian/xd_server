#ifndef XD_SOCKET_SERVER_H
#define XD_SOCKET_SERVER_H

#include "../base/base_object.h"
#include <map>

class XDSocketAccept;

class XDSocketServer : public XDBaseObject
{
protected:
    enum XDSocketServerState
    {
        ST_INIT,
        ST_INITED,
        ST_STARTED,
        ST_CLOSED
    };
public:
    XDSocketServer();
    virtual ~XDSocketServer();

    virtual bool init(int32 port) = 0;
    virtual bool start();
    virtual bool stop();

    virtual int32 poll();
    virtual int32 poll(int32 timeout);

    virtual bool accept() = 0;
    //virtual bool acceptNewConnection(XDSockFD newConn) = 0;

protected:
    XDSocketAccept serverSocketFD_;
    XDPollFD pollFD_;
    XDSocketServerState state_;
    bool isStop_;
    XDSockFD recvFD_; // 管道接收端
    XDSockFD sendFD_; // 管道发送端
};

#endif // end xd_socket_server_h
