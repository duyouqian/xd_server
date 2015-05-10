#ifndef XD_RPC_MESSAGE_H
#define XD_RPC_MESSAGE_H

#include "message.h"
#include "../base/socket_util.h"

struct XDRpcUnpackedMessageHeader : public XDMessage::XDHeader
{
    uint32 sessionID;
    uint64 replyID;
    uint32 serverID;
    uint32 methodID;
    XDRpcUnpackedMessageHeader(const XDMessage &message)
    {
        const XDMessage::XDHeader &h = message.header();
        cmdID = h.cmdID;
        flags = h.flags;
        bodySize = h.bodySize;
        sessionID = message.sessionID();
        replyID = message.replyID();
        serverID = message.serverID();
        methodID = message.methodID();
    }
};

// request
class XDRpcRequest
{
public:
    XDRpcRequest(uint32 serverID, uint32 methodID, uint32 cmdID, uint32 bodySize, XDReplyMessageHandler *handler = NULL, int32 timeout = -1);
    
    void setReplyID(uint64 value) { replyID_ = value; }
    uint64 replyID() const { return replyID_; }
    XDReplyMessageHandler* replyHandler() { return handler_; }
    int32 timeout() const { return timeout_; }
    
    virtual operator XDMessage& () { return message_; }
    
protected:
    XDMessage message_;
    uint64 replyID_;
    uint32 serverID_;
    uint32 methidID_;
    XDReplyMessageHandler *handler_;
    int32 timeout_;
};

// response
class XDRpcResponse : public XDResponse
{
public:
    XDRpcResponse(uint64 replyID, uint32 bodySize);
};

#endif // end xd_rpc_message_h