#ifndef XD_SOCKET_H
#define XD_SOCKET_H

#include "../base/base_object.h"
#include "../base/socket_util.h"

class XDBaseSocket : public XDBaseObject
{
protected:
    enum XDSocketState
    {
        ST_INIT,
        ST_CONNECTED,
        ST_HALFCLOSE,
        ST_CLOSED
    };

public:
    XDBaseSocket();
    virtual ~XDBaseSocket();

    virtual bool create() = 0;
    // send
    int32 send(void *data, int32 len);
    // read
    int32 read(void *data, int32 len);
    // close
    void close();

protected:
    XDSockFD fd_;
    XDSocketState state_;
    XDSocketUtil::XDSockAddr addr_;
};

#endif // end xd_socket_h
