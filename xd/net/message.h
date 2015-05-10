#ifndef XD_MESSAGE_H
#define XD_MESSAGE_H

#include "../base/types.h"

#define XDMESSAGE_MIN_SIZE uint32(1024)
#define XDMESSAGE_MAX_SIZE uint32(1 * 1024 * 1024)
#define XDMESSAGE_HEADER_SIZE sizeof(XDMessage::XDHeader)

#define XDRESPONSE_MESSAGE_ID 0xFFFFFFFF

#define XDMESSAGE_FLAG_HASSESSIONID     0x0001
#define XDMESSAGE_FLAG_HASRELPID        0x0002
#define XDMESSAGE_FLAG_HASSERVERID      0x0004 // for rpc
#define XDMESSAGE_FLAG_HASMETHODID      0x0008 // for rpc
#define XDMESSAGE_FLAG_INVALID          0x8000

class XDMessage
{
public:
    #pragma pack(push, 1)
    struct XDHeader
    {
        uint32 cmdID;
        uint16 flags;
        uint32 bodySize;
    }; // __attribute__ ((aligned (1)))
    #pragma pack(pop)

public:
    XDMessage();
    XDMessage(uint32 cmdID, uint16 flags, uint32 bodySize);
    XDMessage(const XDMessage& other);
    virtual ~XDMessage();

    virtual bool init(uint32 cmdID, uint16 flags, uint32 bodySize);
    virtual void reset();
    void copy(const XDMessage& other);
    void operator=(const XDMessage& other);

    uint64* sessionIDPtr() { return sessionID_; }
    uint64 sessionID() { return sessionID_ ? *sessionID_ : 0; }
    uint64 sessionID() const { return sessionID_ ? *sessionID_ : 0; }
    void sessionID(uint64 value) { if (sessionID_) *sessionID_ = value; }

    uint64* replyIDPtr() { return replyID_; }
    uint64 replyID() { return replyID_ ? *replyID_ : 0; }
    uint64 replyID() const { return replyID_ ? *replyID_ : 0; }
    void replyID(uint64 value) { if (replyID_) *replyID_ = value; }
    
    uint32* serverIDPtr() { return serverID_; }
    uint32 serverID() { return serverID_ ? *serverID_ : 0; }
    uint32 serverID() const { return serverID_ ? *serverID_ : 0; }
    void serverID(uint32 value) { if (serverID_) *serverID_ = value; }
    
    uint32* methodDPtr() { return methodID_; }
    uint32 methodID() { return methodID_ ? *methodID_ : 0; }
    uint32 methodID() const { return methodID_ ? *methodID_ : 0; }
    void methodID(uint32 value) { if (methodID_) *methodID_ = value; }

    XDHeader& header() { return *reinterpret_cast<XDHeader*>(buf_); }
    XDHeader& header() const { return *reinterpret_cast<XDHeader*>(buf_); }
    char* body();
    char* body() const;
    char* data() { return buf_; }
    uint32 size() const;

    void isHostEndian(bool value) { isHostEndian_ = value; }

public:
    static void parseHeader(const char *buf, XDHeader &h);
    static uint32 optionalHeaderSize(uint16 flags);

protected:
    virtual void initOptionalHeader();
    bool allocateBuf(uint32 size);

protected:
    char *buf_;
    char data_[XDMESSAGE_MIN_SIZE];
    uint64 *sessionID_;
    uint64 *replyID_;
    uint32 *serverID_;
    uint32 *methodID_;
    bool isHostEndian_;
};

class XDReplyMessageHandler;

struct XDUnpackedMessageHeader : public XDMessage::XDHeader
{
    uint32 sessionID;
    uint64 replyID;
    XDUnpackedMessageHeader(const XDMessage &message)
    {
        const XDMessage::XDHeader &h = message.header();
        cmdID = h.cmdID;
        flags = h.flags;
        bodySize = h.bodySize;
        sessionID = message.sessionID();
        replyID = message.replyID();
    }
};

// request
class XDRequest
{
public:
    XDRequest(uint32 cmdID, uint32 bodySize, XDReplyMessageHandler *handler = NULL, int32 timeout = -1);

    void setReplyID(uint64 value) { replyID_ = value; }
    uint64 replyID() const { return replyID_; }
    XDReplyMessageHandler* replyHandler() { return handler_; }
    int32 timeout() const { return timeout_; }

    virtual operator XDMessage& () { return message_; }

protected:
    XDMessage message_;
    uint64 replyID_;
    XDReplyMessageHandler *handler_;
    int32 timeout_;
};

// request
class XDResponse
{
public:
    XDResponse(uint64 replyID, uint32 bodySize);

    uint64 replyID() const { return replyID_; }
    virtual operator XDMessage& () { return message_; }

protected:
    XDMessage message_;
    uint64 replyID_;
};

#endif // end xd_message_h
