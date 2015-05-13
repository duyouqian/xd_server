#include "socket_connection.h"
#include "../base/log.h"
#include "../base/socket_util.h"
#include "../base/guard_mutex.h"
#include "tcp_server.h"
#include <string.h>
#include <errno.h>

XDSocketConnection::XDSocketConnection(XDSocketServer *socketServer, XDSocketType type)
                  : XDBaseSocket::XDBaseSocket(socketServer, type)
                  , readBufWritePos_(NULL)
                  , toRead_(0)
                  , readHandle_(NULL)
                  , curSendMessage_(NULL)
                  , curSendPos_(0)
{
    
}

XDSocketConnection::XDSocketConnection(XDSocketServer *socketServer)
                  : XDBaseSocket::XDBaseSocket(socketServer, XDSocketType::SOCKETTYPE_CONNECTION)
                  , readBufWritePos_(NULL)
                  , toRead_(0)
                  , readHandle_(NULL)
                  , curSendMessage_(NULL)
                  , curSendPos_(0)
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

// close
void XDSocketConnection::onClose()
{
    connDisconnectCallBack();
    //server_.connDisconnectCallBack(handle_);
}

void XDSocketConnection::nextReadStep(char *buf,
                                      int32 size,
                                      XDReadHandle handle)
{
    readBufWritePos_ = buf;
    toRead_ = size;
    readHandle_ = handle;
}

int32 XDSocketConnection::onRead()
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
            XD_LOG_merror("[XDSocketConnection] [Handle:%d fd:%d] Read errno:%d error:%s",
                          handle_, fd_, errno, strerror(errno));
            if (errno == EINTR) {
                continue;
            }
            if (errno == EAGAIN) {
                // 已经到末尾了
                return -1;
            }
            if (errno == EWOULDBLOCK) {
                // 已经到末尾了
                return -1;
            }
            return -1;
        } else if (0 == readNum) {
            // close
            XD_LOG_merror("[XDSocketConnection] [Handle:%d fd:%d] close",
                          handle_, fd_);
            connDisconnectCallBack();
            //server_.connDisconnectCallBack(handle_);
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
    if (message_.sessionIDPtr()) {
        message_.sessionID(XDSocketUtil::networkToHost64(message_.sessionID()));
    }
    if (message_.replyIDPtr()) {
        message_.replyID(XDSocketUtil::networkToHost64(message_.replyID()));
    }
    if (message_.serverIDPtr()) {
        message_.serverID(XDSocketUtil::networkToHost32(message_.serverID()));
    }
    if (message_.methodDPtr()) {
        message_.methodID(XDSocketUtil::networkToHost32(message_.methodID()));
    }
    // process message
    connMessageCallBack();
    //server_.connMessageCallBack(handle_, message_);
    //((XDNetService*)server_)->processMessage(this, message_);
    message_.reset();
    nextReadStep(messageHeader_, XDMESSAGE_HEADER_SIZE, &XDSocketConnection::handleReadMessageHeader);
    return true;
}

bool XDSocketConnection::send(XDMessage &message)
{
    if (state_ != ST_CONNECTED) {
        return false;
    }
    if (message.header().flags & XDMESSAGE_FLAG_INVALID) {
        return false;
    }
    {
        XDGuardMutex lock(&sendMutex_);
        sendMessageQueue_.push_back(message);
    }
    connSendMessageCallBack(true);
    //server_.connSendMessageCallBack(handle_, true);
    return true;
}


bool XDSocketConnection::send(XDRequest &request)
{
    if (request.replyHandler()) {
        request.setReplyID(socketServer_->getNextReplyID());
    }
    bool ret = send((XDMessage&)request);
    if (ret) {
        socketServer_->registerReplyRecord(request.replyID(), request.replyHandler(), request.timeout());
    }
    return ;
}

bool XDSocketConnection::send(XDResponse &response)
{
    return send((XDMessage&)response);
}

void XDSocketConnection::onSend()
{
    if (NULL == curSendMessage_ ||
        curSendPos_ == curSendMessage_->size()) {
        bool ret = getNextSendMessageByQueue();
        if (!ret) {
            // 没有消息
            connSendMessageCallBack(false);
            //server_.connSendMessageCallBack(handle_, false);
            return;
        }
    }
    // send
    int32 ret = XDBaseSocket::send(curSendMessage_->data() + curSendPos_, curSendMessage_->size() - curSendPos_);
    if (0 == ret) {
        // close
        //server_.connDisconnectCallBack(handle_);
        connDisconnectCallBack();
    } else if (ret < 0) {
        XD_LOG_merror("[XDSocketConnection] [Handle:%d fd:%d] Send error:%s",
                      handle_, fd_, strerror(errno));
        if (errno == EAGAIN || errno == EINTR || errno == EWOULDBLOCK) {
            
        } else {
            //server_.connDisconnectCallBack(handle_);
            connDisconnectCallBack();
        }
    } else {
        curSendPos_ += ret;
        if (curSendPos_ == curSendMessage_->size()) {
            {
                XDGuardMutex lock(&sendMutex_);
                sendMessageQueue_.pop_front();
                curSendMessage_ = NULL;
                curSendPos_ = 0;
            }
            
        }
    }
}

bool XDSocketConnection::getNextSendMessageByQueue()
{
    XDGuardMutex lock(&sendMutex_);
    if (sendMessageQueue_.empty()) {
        return false;
    }
    curSendMessage_ = &sendMessageQueue_.front();
    curSendPos_ = 0;
    return true;
}

void XDSocketConnection::connMessageCallBack()
{
    if (socketServer_) {
        socketServer_->connMessageCallBack(handle_, message_);
    }
}

void XDSocketConnection::connDisconnectCallBack()
{
    if (socketServer_) {
        socketServer_->connDisconnectCallBack(handle_);
    }
}

void XDSocketConnection::connSendMessageCallBack(bool enable)
{
    if (socketServer_) {
        socketServer_->connSendMessageCallBack(handle_, enable);
    }
}
