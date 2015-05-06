#include "socket_server.h"
#include "socket_acceptor.h"
#include "socket_poll.h"
#include "../base/log.h"
#include <string.h>
#include <errno.h>

XDSocketServer::XDSocketServer()
              : state_(ST_INIT)
              , isStop_(true)
{

}

XDSocketServer::~XDBaseObject()
{
    stop();
}

bool XDSocketServer::start()
{
    XD_LOG_mdebug("[SocketServer] SocketServer start");
    if (state_ == ST_INITED || state_ == ST_CLOSED) {
        bool ret = false;
        do {
            // 创建接受/发送消息FD
            XDSockFD fd[2];
            pollFD_ = XDSocketPoll::create();
            ret = !XDSocketPoll::isInvalid(pollFD_);
            XD_LOG_mdebug("[SocketServer] 开始创建poll");
            if (!ret) {
                XD_LOG_merror("[SocketServer] 创建poll 失败");
                break;
            }
            XD_LOG_mdebug("[SocketServer] 开始创建pipe");
            ret = -1 != pipe(fd);
            if (!ret) {
                XD_LOG_merror("[SocketServer] 创建pipe 失败");
                XDSocketPoll::release(pollFD_);
                break;
            }
            // 添加接收端到poll
            XD_LOG_mdebug("[SocketServer] 开始添加pipe接收端到poll");
            ret = 0 == XDSocketPoll::add(pollFD_, fd[0], &recvFD_);
            if (!ret) {
                XD_LOG_merror("[SocketServer] 添加pipe接收端到poll 失败");
                XDSocketUtil::close(fd[0]);
                XDSocketUtil::close(fd[1]);
                XDSocketPoll::release(pollFD_);
                break;
            }
            XD_LOG_mdebug("[SocketServer] 开始创建 socket_acceptor");
            ret = serverSocketFD_.create();
            if (!ret) {
                XD_LOG_merror("[SocketServer] 创建socket_acceptor 失败");
                XDSocketUtil::close(fd[0]);
                XDSocketUtil::close(fd[1]);
                XDSocketPoll::release(pollFD_);
                break;
            }
            XD_LOG_mdebug("[SocketServer] 开始添加socketAcceptor到poll");
            ret = 0 == XDSocketPoll::add(pollFD_, serverSocketFD_, NULL);
            if (!ret) {
                XD_LOG_merror("[SocketServer] 添加socketAcceptor到poll 失败");
                serverSocketFD_.close();
                XDSocketUtil::close(fd[0]);
                XDSocketUtil::close(fd[1]);
                XDSocketPoll::release(pollFD_);
                break;
            }
            state_ = ST_STARTED;
            isStop_ = false;
            XD_LOG_mdebug("[SocketServer] SocketServer start succ");
            return true;
        } while (0);
    }
    XD_LOG_mdebug("[SocketServer] SocketServer start failure");
    return false;
}

bool XDSocketServer::stop()
{
    if (state_ != ST_STARTED) {
        return false;
    }
    isStop_ = true;
    state_ = ST_CLOSED;
    XDSocketPoll::del(pollFD_, serverSocketFD_.fd());
    serverSocketFD_.close();
    XDSocketUtil::close(recvFD_);
    XDSocketUtil::close(sendFD_);
}

int32 XDSocketServer::poll()
{
    return poll(-1);
}

int32 XDSocketServer::poll(int32 timeout)
{
    if (state_ != ST_STARTED) {
        return -1;
    }
    XDSocketPoll::XDEvent event[128];
    int32 num;
    int32 i;
    while (!isStop_) {
        num = XDSocketPoll::wait(pollFD_, event, 128);
        if (-1 == num) {
            XD_LOG_merror("[SocketServer] poll错误 error:%s", strerror(errno));
            continue;
        }
        for (i = 0; i < num; ++i) {
            accept();
        }
    }
    stop();
}
