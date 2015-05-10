#ifndef XD_SOCKET_H
#define XD_SOCKET_H

#include "../base/socket_addr.h"
#include "../base/refcount.h"

class XDTcpServer;

class XDBaseSocket : public XDRefCounted
{
    friend class XDTcpServer;
private:
    static XDHandle sHandle_;
protected:
    enum XDSocketState
    {
        ST_INIT,
        ST_CONNECTING,
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
    void setFD(XDSockFD fd) { fd_ = fd; }
    XDHandle handle() const { return handle_; }
    XDSocketUtil::XDSockAddr& address() { return addr_; }
    XDSocketUtil::XDSockAddr* addressPtr() { return &addr_; }
    
protected:
    virtual void connMessageCallBack() { }
    virtual void connDisconnectCallBack() { }
    virtual void connSendMessageCallBack(bool enable) { }

protected:
    XDSockFD fd_;
    XDSocketState state_;
    XDSocketUtil::XDSockAddr addr_;
    XDHandle handle_;
};

#endif // end xd_socket_h
