#include "event_loop.h"
#include "socket_poll.h"
#include "../base/socket_util.h"
#include "../base/log.h"
#include "socket.h"
#include "socket_acceptor.h"
#include "socket_connection.h"
#include "socket_connecter.h"
#include <string.h>
#include <errno.h>

XDEventLoop::XDEventLoop(const char *name)
           : name_(name)
           , pollFD_(-1)
           , isStop_(true)
           , thread_()
{
    std::string threadName("thread-");
    threadName.append(name);
    thread_.setName(threadName.c_str());
}

XDEventLoop::XDEventLoop()
           : name_("EventLoop")
           , pollFD_(-1)
           , isStop_(true)
           , thread_()
{
    std::string threadName("thread-");
    threadName.append(name_);
    thread_.setName(threadName.c_str());
}

XDEventLoop::~XDEventLoop()
{
    XDSocketUtil::close(recvFD_);
    XDSocketUtil::close(sendFD_);
    XDSocketPoll::release(pollFD_);
}

void XDEventLoop::setName(const char *name)
{
    name_.assign(name);
    std::string threadName("thread-");
    threadName.append(name);
    thread_.setName(threadName.c_str());
}

bool XDEventLoop::init()
{
    // 创建管道
    XDSockFD fd[2];
    if (-1 == pipe(fd)) {
        return false;
    }
    pollFD_ = XDSocketPoll::create();
    if (XDSocketPoll::isInvalid(pollFD_)) {
        XDSocketUtil::close(fd[0]);
        XDSocketUtil::close(fd[1]);
        return false;
    }
    // 添加接收端到poll
    if (!add(fd[0], &recvFD_)) {
        XDSocketUtil::close(fd[0]);
        XDSocketUtil::close(fd[1]);
        XDSocketPoll::release(pollFD_);
        return false;
    }
    recvFD_ = fd[0];
    sendFD_ = fd[1];
    return true;
}

bool XDEventLoop::start()
{
    if (XDSocketPoll::isInvalid(pollFD_)) {
        return false;
    }
    isStop_ = false;
    thread_.start(this);
    thread_.detach();
    return true;
}

bool XDEventLoop::stop()
{
    isStop_ = true;
    return true;
}

bool XDEventLoop::run()
{
    XDSocketPoll::XDEvent event[128];
    int32 num;
    int32 i;
    while (!isStop_) {
        num = XDSocketPoll::wait(pollFD_, event, 128);
        if (-1 == num) {
            if (EINTR == errno) {
                continue;
            }
            XD_LOG_merror("[EventLoop] poll错误 error:%s", strerror(errno));
            continue;
        }
        for (i = 0; i < num; ++i) {
            XDBaseSocket *sock = (XDBaseSocket *)event[i].ptr;
            if (sock->type() == XDSocketType::SOCKETTYPE_ACCEPTOR) {
                onAcceptHandler(&event[i]);
            } else if (event[i].ptr == &recvFD_) {
                if (event[i].read) {
                    uint64 one = 1;
                    int32 n = XDSocketUtil::read(recvFD_, &one, sizeof(one));
                    if (n != sizeof(one)) {
                        XD_LOG_merror("[EventLoop] reads:%d bytes instead of 8", n);
                    }
                }
            } else {
                if (event[i].read) {
                    onReadHandler(&event[i], sock->type());
                }
                if (event[i].write) {
                    onWriteHandler(&event[i], sock->type());
                }
            }
        }
    }
    isStop_ = true;
    return true;
}

bool XDEventLoop::add(XDSockFD fd, void *ud)
{
    return 0 == XDSocketPoll::add(pollFD_, fd, ud);
}

bool XDEventLoop::del(XDSockFD fd)
{
    XDSocketPoll::del(pollFD_, fd);
    return true;
}

bool XDEventLoop::write(XDSockFD fd, void *ud, bool enable)
{
    XDSocketPoll::write(pollFD_, fd, ud, enable);
    return true;
}

bool XDEventLoop::onAcceptHandler(XDSocketPoll::XDEvent *event)
{
    XDSocketAccept *accept = (XDSocketAccept*)event->ptr;
    accept->accept();
    return true;
}

// TODO 可优化
bool XDEventLoop::onReadHandler(XDSocketPoll::XDEvent *event, XDSocketType type)
{
    if (type == XDSocketType::SOCKETTYPE_CONNECTION) {
        XDSocketConnection *conn = (XDSocketConnection *)event->ptr;
        conn->onRead();
    } else if (type == XDSocketType::SOCKETTYPE_CONNECTER) {
        XDSocketConnecter *conn = (XDSocketConnecter *)event->ptr;
        conn->onRead();
    }
    return true;
}

bool XDEventLoop::onWriteHandler(XDSocketPoll::XDEvent *event, XDSocketType type)
{
    if (type == XDSocketType::SOCKETTYPE_CONNECTION) {
        XDSocketConnection *conn = (XDSocketConnection *)event->ptr;
        conn->onSend();
    } else if (type == XDSocketType::SOCKETTYPE_CONNECTER) {
        XDSocketConnecter *conn = (XDSocketConnecter *)event->ptr;
        conn->onSend();
    }
    return true;
}

void XDEventLoop::wakeup()
{
    // 唤醒poll
    XD_LOG_mdebug("[EventLoop] 唤醒");
    uint64 one = 1;
    int32 n = XDSocketUtil::write(sendFD_, &one, sizeof(one));
    if (n != sizeof(one)) {
        XD_LOG_merror("[EventLoop] reads:%d bytes instead of 8", n);
    }
}
