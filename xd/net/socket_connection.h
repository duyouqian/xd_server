#ifndef XD_SOCKET_CONNECTION_H
#define XD_SOCKET_CONNECTION_H

#include "socket.h"
#include "message.h"

class XDTcpServer;

class XDSocketConnection : public XDBaseSocket
{
public:
    XDSocketConnection(XDTcpServer &server);
    ~XDSocketConnection();

    virtual bool create();

    void setWorkThreadIndex(int32 index) { workThreadIndex_ = index; }
    int32 workThreadIndex() const { return workThreadIndex_; }

    int32 read();
    void onClose();

protected:
    typedef bool(XDSocketConnection::*XDReadHandle)();
    int32 processReadBuff(const void *data, int32 len);
    void nextReadStep(char *buf, int32 size, XDReadHandle handle);
    bool handleReadMessageHeader();
    bool handleReadMessagePayload();
    int32 processReadBuffer();

protected:
    XDTcpServer &server_;
    int32 workThreadIndex_;
    char messageHeader_[XDMESSAGE_HEADER_SIZE];
    XDMessage message_;
    char *readBufWritePos_;
    int32 toRead_;
    XDReadHandle readHandle_;
};

#endif // end xd_socket_connection_h
