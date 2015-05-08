#include "socket_server.h"
#include "socket_poll.h"
#include "../base/socket_util.h"
#include "../base/log.h"
#include "../base/guard_mutex.h"
#include "../base/base_thread.h"
#include <string.h>
#include <errno.h>

XDHandle XDSocketServer::sHandle_ = 0;

XDSocketServer::XDSocketServer()
              : state_(ST_INIT)
              , isStop_(true)
              , callingPendingFunctors_(false)
              , callingPendingTasks_(false)
              , threadID_(XDBaseThread::getCurrentThreadID())
              , handle_(++sHandle_)
{

}

XDSocketServer::~XDSocketServer()
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
                XDSocketPoll::del(pollFD_, fd[0]);
                XDSocketUtil::close(fd[0]);
                XDSocketUtil::close(fd[1]);
                XDSocketPoll::release(pollFD_);
                break;
            }
            XD_LOG_mdebug("[SocketServer] 开始添加socketAcceptor到poll");
            ret = 0 == XDSocketPoll::add(pollFD_, serverSocketFD_.fd(), NULL);
            if (!ret) {
                XD_LOG_merror("[SocketServer] 添加socketAcceptor到poll 失败");
                serverSocketFD_.close();
                XDSocketPoll::del(pollFD_, fd[0]);
                XDSocketUtil::close(fd[0]);
                XDSocketUtil::close(fd[1]);
                XDSocketPoll::release(pollFD_);
                break;
            }
            recvFD_ = fd[0];
            sendFD_ = fd[1];
            state_ = ST_STARTED;
            isStop_ = false;
            XD_LOG_mdebug("[SocketServer] SocketServer start succ 监听地址:%s:%d", serverSocketFD_.address().ip, serverSocketFD_.address().port);
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
    return true;
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
            if (EINTR == errno) {
                goto exceFunc;
            }
            XD_LOG_merror("[SocketServer] poll错误 error:%s", strerror(errno));
            goto exceFunc;
        }
        for (i = 0; i < num; ++i) {
            if (NULL == event[i].ptr) {
                accept();
            } else if (event[i].ptr == &recvFD_){
                if (event[i].read) {
                    uint64 one = 1;
                    int32 n = XDSocketUtil::read(recvFD_, &one, sizeof(one));
                    if (n != sizeof(one)) {
                        XD_LOG_merror("[SocketServer] reads:%d bytes instead of 8", n);
                    }
                }
                XD_LOG_mdebug("[SocketServer] 接收管道被激活");
            }
        }
     exceFunc:
        // 执行其他线程过来的
        doPendingFunctors();
        // 执行io线程来的任务
        doPendingTask();
    }
    stop();
    return 0;
}

void XDSocketServer::wakeup()
{
    // 唤醒poll
    XD_LOG_mdebug("[SocketServer] 唤醒");
    uint64 one = 1;
    int32 n = XDSocketUtil::write(sendFD_, &one, sizeof(one));
    if (n != sizeof(one)) {
        XD_LOG_merror("[SocketServer] reads:%d bytes instead of 8", n);
    }
}

void XDSocketServer::doPendingFunctors()
{
    std::vector<XDFunction> temp;
    callingPendingFunctors_ = true;
    if (pendingFunctors_.empty()) {
        callingPendingFunctors_ = false;
        return;
    }
    {
        XDGuardMutex lock(&mutex_);
        temp.swap(pendingFunctors_);
    }
    for (int32 i = 0, len = temp.size(); i < len; ++i) {
        temp[i]();
    }
    callingPendingFunctors_ = false;
}

void XDSocketServer::doPendingTask()
{
    std::vector<XDITask*> temp;
    callingPendingTasks_ = true;
    if (pendingTask_.empty()) {
        callingPendingTasks_ = false;
        return;
    }
    {
        XDGuardMutex lock(&mutex_);
        temp.swap(pendingTask_);
    }
    for (int32 i = 0, len = temp.size(); i < len; ++i) {
        (*temp[i])();
        delete temp[i];
    }
    callingPendingTasks_ = false;
}

void XDSocketServer::runInLoop(const XDFunction &cb)
{
    if (isInLoopThread()) {
        cb();
    } else {
        queueInLoop(cb);
    }
}

void XDSocketServer::addTask(XDITask* task)
{
    {
        XDGuardMutex lock(&mutex_);
        pendingTask_.push_back(task);
    }
    if (!isInLoopThread() || callingPendingTasks_) {
        wakeup();
    }
}

void XDSocketServer::queueInLoop(const XDFunction &cb)
{
    {
        XDGuardMutex lock(&mutex_);
        pendingFunctors_.push_back(cb);
    }
    if (!isInLoopThread() || callingPendingFunctors_) {
        wakeup();
    }
}

FROCEINLINE bool XDSocketServer::isInLoopThread()
{
    return threadID_ == XDBaseThread::getCurrentThreadID();
}
