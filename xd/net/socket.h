#ifndef XD_SOCKET_H
#define XD_SOCKET_H

#include "../base/socket_util.h"
#include "../base/refcount.h"

class XDBaseSocket : public XDRefCounted
{
private:
    static XDHandle sHandle_;
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
    virtual int32 send(void *data, int32 len);
    // read
    virtual int32 read(void *data, int32 len);
    // close
    virtual void close();

    XDSockFD fd() const { return fd_; }
    XDHandle handle() const { return handle_; }

protected:
    XDSockFD fd_;
    XDSocketState state_;
    XDSocketUtil::XDSockAddr addr_;
    XDHandle handle_;
};

#endif // end xd_socket_h
