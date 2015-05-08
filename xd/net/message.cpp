#include "message.h"
#include "../base/socket_util.h"
#include "../base/log.h"

#include <string.h>

XDMessage::XDMessage()
         : buf_(data_)
         , sessionID_(NULL)
         , replyID_(NULL)
         , isHostEndian_(true)
{
    memset(data_, 0, sizeof(data_));
}

XDMessage::XDMessage(uint32 cmdID, uint16 flags, uint32 bodySize)
         : buf_(data_)
         , sessionID_(NULL)
         , replyID_(NULL)
{
    memset(data_, 0, sizeof(data_));
    init(cmdID, flags, bodySize);
}

XDMessage::XDMessage(const XDMessage &other)
         : buf_(data_)
         , sessionID_(NULL)
         , replyID_(NULL)
{
    copy(other);
}

XDMessage::~XDMessage()
{

}

void XDMessage::copy(const XDMessage &other)
{
    XD_LOG_mdebug("[Message] copy");
    if (other.buf_ == other.data_) {
        buf_ = data_;
        memcpy(buf_, other.buf_, other.size());
    } else {
        buf_ = other.buf_;
    }
    initOptionalHeader();
}

void XDMessage::operator=(const XDMessage &other)
{
    copy(other);
}

void XDMessage::parseHeader(const char *buf, XDHeader &h)
{
    h.cmdID = *(uint32*)buf;
    h.cmdID = XDSocketUtil::networkToHost32(h.cmdID);
    buf += sizeof(uint32);
    h.flags = *(uint16*)buf;
    h.flags = XDSocketUtil::networkToHost16(h.flags);
    buf += sizeof(uint16);
    h.bodySize = *(uint32*)buf;
    h.bodySize = XDSocketUtil::networkToHost32(h.bodySize);
}

uint32 XDMessage::optionalHeaderSize(uint16 flags)
{
    uint32 size = 0;
    if (flags & XDMESSAGE_FLAG_HASSESSIONID) {
        size += (uint32)(sizeof(uint64));
    }
    if (flags & XDMESSAGE_FLAG_HASRELPID) {
        size += (uint32)(sizeof(uint64));
    }
    return size;
}

bool XDMessage::init(uint32 cmdID, uint16 flags, uint32 bodySize)
{
    if (bodySize > XDMESSAGE_MAX_SIZE) {
        return false;
    }
    uint32 optHeadSize = optionalHeaderSize(flags);
    if (!allocateBuf(XDMESSAGE_HEADER_SIZE + optHeadSize + bodySize)) {
        return false;
    }
    header().cmdID = cmdID;
    header().flags = flags;
    header().bodySize = bodySize;
    initOptionalHeader();

    if (sessionID_) {
        *sessionID_ = 0;
    }
    if (replyID_) {
        *replyID_ = 0;
    }
    return true;
}

void XDMessage::reset()
{
    buf_ = data_;
    sessionID_ = NULL;
    replyID_ = NULL;
    isHostEndian_ = true;
}

char* XDMessage::body()
{
    return buf_ + sizeof(XDHeader) + optionalHeaderSize(header().flags);
}

char* XDMessage::body() const
{
    return buf_ + sizeof(XDHeader) + optionalHeaderSize(header().flags);
}

uint32 XDMessage::size() const
{
    return sizeof(XDHeader) + optionalHeaderSize(header().flags) + (isHostEndian_ ? header().bodySize : XDSocketUtil::networkToHost32(header().bodySize));
}

void XDMessage::initOptionalHeader()
{
    char *payload = buf_ + sizeof(XDHeader);
    int32 offset = 0;
    if (header().flags & XDMESSAGE_FLAG_HASSESSIONID) {
        sessionID_ = reinterpret_cast<uint64*>(payload);
        offset += sizeof(uint64);
    }
    if (header().flags & XDMESSAGE_FLAG_HASRELPID) {
        replyID_ = reinterpret_cast<uint64*>(payload);
        offset += sizeof(uint64);
    }
}

bool XDMessage::allocateBuf(uint32 size)
{
    // TODO
    buf_ = data_;
    return true;
}

XDRequest::XDRequest(uint32 cmdID,
                     uint32 bodySize,
                     XDReplyMessageHandler *handler,
                     int32 timeout)
         : message_(cmdID, handler ? XDMESSAGE_FLAG_HASRELPID : 0, bodySize)
         , handler_(handler)
         , timeout_(timeout)
{

}

XDResponse::XDResponse(uint64 replyID,
                       uint32 bodySize)
          : message_(XDRESPONSE_MESSAGE_ID, XDMESSAGE_FLAG_HASRELPID, bodySize)
          , replyID_(replyID)
{

}
