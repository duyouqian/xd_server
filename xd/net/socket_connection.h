#ifndef XD_SOCKET_CONNECTION_H
#define XD_SOCKET_CONNECTION_H

#include "socket.h"
#include "message.h"
#include "../base/mutex.h"
#include <deque>

class XDTcpServer;

class XDSocketConnection : public XDBaseSocket
{
public:
    XDSocketConnection();
    XDSocketConnection(XDTcpServer *server);
    ~XDSocketConnection();

    virtual bool create();

    void setWorkThreadIndex(int32 index) { workThreadIndex_ = index; }
    int32 workThreadIndex() const { return workThreadIndex_; }

    // read
    int32 read();
    
    // close
    void onClose();
    
    // send
    bool send(XDMessage &message);
    // in networkt thread
    void onSend();

protected:
    // read
    typedef bool(XDSocketConnection::*XDReadHandle)();
    int32 processReadBuff(const void *data, int32 len);
    void nextReadStep(char *buf, int32 size, XDReadHandle handle);
    bool handleReadMessageHeader();
    bool handleReadMessagePayload();
    int32 processReadBuffer();
    
    // send
    bool getNextSendMessageByQueue();
    
protected:
    virtual void connMessageCallBack();
    virtual void connDisconnectCallBack();
    virtual void connSendMessageCallBack(bool enable);

protected:
    XDTcpServer *server_;
    int32 workThreadIndex_;
    
    // read
    char messageHeader_[XDMESSAGE_HEADER_SIZE];
    XDMessage message_;
    char *readBufWritePos_;
    int32 toRead_;
    XDReadHandle readHandle_;
    
    // send
    XDMutex sendMutex_;
    std::deque<XDMessage> sendMessageQueue_;
    XDMessage *curSendMessage_; // 当前发送的消息对象
    int32 curSendPos_; // 当前发送的消息对象的位置
};

#endif // end xd_socket_connection_h
