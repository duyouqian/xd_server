#ifndef XD_SOCKET_H
#define XD_SOCKET_H

#include "../base/socket_addr.h"
#include "../base/refcount.h"

class XDSocketServer;
class XDMessage;
class XDRequest;
class XDResponse;

class XDBaseSocket : public XDRefCounted
{
    friend class XDTcpServer;
private:
    static XDHandle sHandle_;
public:
    enum XDSocketState
    {
        ST_INIT,
        ST_CONNECTING,
        ST_CONNECTED,
        ST_HALFCLOSE,
        ST_CLOSED
    };

public:
    XDBaseSocket(XDSocketServer *socketServer, XDSocketType type);
    virtual ~XDBaseSocket();

    virtual bool create() = 0;
    // send
    virtual int32 send(void *data, int32 len);
    virtual bool send(XDMessage &message) { return false; }
    virtual bool send(XDRequest &request) { return false; }
    virtual bool send(XDResponse &response) { return false; }
    // read
    virtual int32 read(void *data, int32 len);
    // close
    virtual void close();

    XDSockFD fd() const { return fd_; }
    void setFD(XDSockFD fd) { fd_ = fd; }
    XDHandle handle() const { return handle_; }
    XDSocketUtil::XDSockAddr& address() { return addr_; }
    XDSocketUtil::XDSockAddr* addressPtr() { return &addr_; }
    XDSocketType type() const { return type_; }
    XDSocketState state() const { return state_; }

    void setWorkThreadIndex(int32 index) { workThreadIndex_ = index; }
    int32 workThreadIndex() const { return workThreadIndex_; }
    
protected:
    virtual void connMessageCallBack() { }
    virtual void connDisconnectCallBack() { }
    virtual void connSendMessageCallBack(bool enable) { }

protected:
    XDSocketServer *socketServer_;
    XDSocketType type_;
    XDSockFD fd_;
    XDSocketState state_;
    XDSocketUtil::XDSockAddr addr_;
    XDHandle handle_;
    int32 workThreadIndex_;
};

#endif // end xd_socket_h
