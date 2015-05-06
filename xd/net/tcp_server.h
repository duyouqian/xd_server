#ifndef XD_TCP_SERVER_H
#define XD_TCP_SERVER_H

#include "socket_server.h"
#include <memory>
#include <map>

class XDSocketConnection;

class XDTcpServer : public XDSocketServer
{
public:
    typedef std::shared_ptr<XDSocketConnection> connPtr;
    XDTcpServer();
    ~XDTcpServer();

    bool init(int32 port);
    virtual bool accept();
    bool disconnect(XDHandle handle);

protected:
    std::map<XDHandle, connPtr> conns_;
};

#endif // end xd_tcp_server_h
