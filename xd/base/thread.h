#ifndef XD_THREAD_H
#define XD_THREAD_H

#include "base_thread.h"

class XDRunnable;

class XDThread : public XDBaseThread
{
public:
    XDThread();
    XDThread(const char *name);
    ~XDThread();

    void setName(const char *name) { name_.assign(name); }
    bool run();

    bool start(XDRunnable *runnable);

protected:
    XDRunnable *runnable_;
};

#endif // end xd_thread_h
