#include "thread.h"
#include "runnable.h"

XDThread::XDThread(const char *name)
        : XDBaseThread::XDBaseThread(name)
{

}

XDThread::XDThread()
        : XDBaseThread::XDBaseThread("Thread")
{

}

XDThread::~XDThread()
{
    join();
}

bool XDThread::run()
{
    bool ret = true;
    if (runnable_) {
        ret = runnable_->run();
    }
    return ret;
}

bool XDThread::start(XDRunnable *runnable)
{
    runnable_ = runnable;
    XDBaseThread::start();
    return true;
}
