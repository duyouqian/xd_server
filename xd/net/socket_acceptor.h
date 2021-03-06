#ifndef XD_SOCKET_ACCEPTOR_H
#define XD_SOCKET_ACCEPTOR_H

#include "socket.h"

// socket server accept

class XDSocketAccept : public XDBaseSocket
{
public:
    XDSocketAccept(XDSocketServer *socketServer);
    explicit XDSocketAccept(XDSocketServer *socketServer, const char *ip, int32 port);
    explicit XDSocketAccept(XDSocketServer *socketServer, int32 port);
    ~XDSocketAccept();
    virtual bool create();

    bool init(const char *ip, int32 port);
    bool init(int32 port);
    XDSockFD accept(XDSocketUtil::XDSockAddr *addr);
    void accept();
};


#endif // end xd_socket_acceptor_h
