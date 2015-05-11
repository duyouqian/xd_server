#include "io_service.h"
#include "../base/interface.h"
#include "../base/guard_mutex.h"
#include "../base/base_thread.h"

XDIOService::XDIOService()
           : event_(true)
           , threadID_(XDBaseThread::getCurrentThreadID())
{

}

XDIOService::~XDIOService()
{

}

void XDIOService::run()
{
    event_.wait();
    doPendingTask();
}

void XDIOService::wakeup()
{
    event_.set();
}

void XDIOService::postTask(XDITask *task)
{
    {
        XDGuardMutex lock(&taskMutex_);
        pendingTask_.push_back(task);
    }
    if (!isInLoopThread() || callingPendingTasks_) {
        event_.set();
    }
}

void XDIOService::doPendingTask()
{
    std::vector<XDITask*> temp;
    callingPendingTasks_ = true;
    if (pendingTask_.empty()) {
        callingPendingTasks_ = false;
        return;
    }
    {
        XDGuardMutex lock(&taskMutex_);
        temp.swap(pendingTask_);
    }
    for (int32 i = 0, len = temp.size(); i < len; ++i) {
        (*temp[i])();
        delete temp[i];
    }
    callingPendingTasks_ = false;
}

FROCEINLINE bool XDIOService::isInLoopThread()
{
    return threadID_ == XDBaseThread::getCurrentThreadID();
}
