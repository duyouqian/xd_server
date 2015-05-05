#ifndef XD_SERVER_SOCKET_H
#define XD_Server_SOCKET_H

#include "socket.h"

class XDServerSocket : public XDBaseSocket
{
public:
    XDServerSocket();
    XDServerSocket(const char *ip, int32 port);
    XDServerSocket(XDSocketUtil::XDSockAddr &addr);
    ~XDServerSocket();
};

#endif // end xd_server_socket_h
