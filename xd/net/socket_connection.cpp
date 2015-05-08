#include "socket_connection.h"
#include "../base/log.h"
#include "../base/socket_util.h"
#include "tcp_server.h"
#include <string.h>
#include <errno.h>

XDSocketConnection::XDSocketConnection(XDTcpServer &server)
                  : XDBaseSocket::XDBaseSocket()
                  , server_(server)
                  , readBufWritePos_(NULL)
                  , toRead_(0)
                  , readHandle_(NULL)
{
    nextReadStep(messageHeader_, XDMESSAGE_HEADER_SIZE,
                 &XDSocketConnection::handleReadMessageHeader);
}

XDSocketConnection::~XDSocketConnection()
{

}

bool XDSocketConnection::create()
{
    if (state_ != ST_INIT && state_ != ST_CLOSED) {
        return false;
    }
    state_ = ST_CONNECTED;
    return true;
}

void XDSocketConnection::nextReadStep(char *buf,
                                      int32 size,
                                      XDReadHandle handle)
{
    readBufWritePos_ = buf;
    toRead_ = size;
    readHandle_ = handle;
}

int32 XDSocketConnection::read()
{
    int32 ret = 0;
    while (1) {
        ret = processReadBuffer();
        if (-1 == ret) {
            // 错误
            nextReadStep(messageHeader_, XDMESSAGE_HEADER_SIZE,
                         &XDSocketConnection::handleReadMessageHeader);
            return -1;
        } else if (0 == ret) {
            // 关闭
            return 0;
        }
    }
    return 1;
}

// return -1:错误, 0:关闭 1:正常
int32 XDSocketConnection::processReadBuffer()
{
    int32 pos = 0;
    while (1) {
        while (!toRead_) {
            // 读取完毕
            if (!(this->*readHandle_)()) {
                return -1;
            }
            return 1;
        }
        int32 readNum = XDBaseSocket::read(readBufWritePos_, toRead_);
        if (-1 == readNum) {
            // EINTR, EAGAIN
            XD_LOG_merror("[XDSocketConnection] [Handle:%d fd:%d] Read error:%s",
                          handle_, fd_, strerror(errno));
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN) {
                // 已经到末尾了
                return -1;
            }
            return -1;
        } else if (0 == readNum) {
            // close
            XD_LOG_merror("[XDSocketConnection] [Handle:%d fd:%d] close",
                          handle_, fd_);
            server_.connDisconnectCallBack(handle_);
            return 0;
        }
        toRead_ -= readNum;
    }
}

bool XDSocketConnection::handleReadMessageHeader()
{
    XDMessage::XDHeader header;
    XDMessage::parseHeader(messageHeader_, header);
    char *payload = NULL;
    int32 payloadSize = 0;
    if (header.bodySize > XDMESSAGE_MAX_SIZE) {
        goto EXIT0;
    }

    message_.reset();
    if (!message_.init(header.cmdID, header.flags, header.bodySize)) {
        goto EXIT0;
    }
    payloadSize = XDMessage::optionalHeaderSize(header.flags) + header.bodySize;
    payload = message_.data() + XDMESSAGE_HEADER_SIZE;
    if (!payloadSize) {
        handleReadMessagePayload();
    } else {
        nextReadStep(payload, payloadSize, &XDSocketConnection::handleReadMessagePayload);
    }
    return true;
EXIT0:
    readHandle_ = NULL;
    return false;
}

bool XDSocketConnection::handleReadMessagePayload()
{
    if (message_.sessionID()) {
        message_.sessionID(XDSocketUtil::networkToHost64(message_.sessionID()));
    }
    if (message_.replyID()) {
        message_.replyID(XDSocketUtil::networkToHost64(message_.replyID()));
    }
    // process message
    server_.connMessageCallBack(handle_, message_);
    //((XDNetService*)server_)->processMessage(this, message_);
    message_.reset();
    nextReadStep(messageHeader_, XDMESSAGE_HEADER_SIZE, &XDSocketConnection::handleReadMessageHeader);
    return true;
}
