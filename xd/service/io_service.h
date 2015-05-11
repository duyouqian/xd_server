#ifndef XD_IO_SERVICE_H
#define XD_IO_SERVICE_H

#include "../base/base_object.h"
#include "../base/event.h"

#include <vector>

class XDITask;

class XDIOService : public XDBaseObject
{
public:
    XDIOService();
    ~XDIOService();

    void run();
    void postTask(XDITask *task);
    void wakeup();

protected:
    void doPendingTask();
    bool isInLoopThread();

private:
    XDSyncEvent event_;
    XDTID threadID_;
    XDMutex taskMutex_;
    // task
    bool callingPendingTasks_;
    std::vector<XDITask*> pendingTask_; // 任务队列
};

#endif // end xd_io_service_h
