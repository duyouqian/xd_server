#ifndef XD_THREAD_H
#define XD_THREAD_H

#include "base_thread.h"
#include "event.h"

class XDRunnable;

class XDThread : public XDBaseThread
{
public:
    XDThread();
    XDThread(const char *name);
    ~XDThread();

    void setName(const char *name) { name_.assign(name); }
    bool beforeRun();
    bool run();

    bool start(XDRunnable *runnable);

protected:
    XDRunnable *runnable_;
    XDSyncEvent startEvent_;
};

#endif // end xd_thread_h
