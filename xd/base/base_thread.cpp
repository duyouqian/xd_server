#include "base_thread.h"
#include <sys/syscall.h>
#include <unistd.h>

static void* threadRunProxy(void *argv)
{
    XDBaseThread *t = (XDBaseThread *)argv;
    t->beforeRun();
    t->run();
    t->runAfter();
    return NULL;
}

uint32 XDBaseThread::getCurrentThreadID()
{
    pid_t id = static_cast<pid_t>(syscall(SYS_gettid));
    return static_cast<uint32>(id);
}

XDBaseThread::XDBaseThread()
            : pid_(NULL)
            , isDetach_(false)
            , name_("Thread-user")
{

}

XDBaseThread::XDBaseThread(const char *name)
            : pid_(NULL)
            , isDetach_(false)
            , name_(name)
{

}

XDBaseThread::~XDBaseThread()
{
    join();
}

bool XDBaseThread::start()
{
    int32 ret = -1;
    if (NULL == pid_) {
        ret = pthread_create(&pid_, NULL, threadRunProxy, this);
    }
    return 0 == ret;
}

bool XDBaseThread::detach()
{
    if (isDetach_) {
        return false;
    }
    if (NULL != pid_) {
        pthread_detach(pid_);
        isDetach_ = true;
        return true;
    }
    return false;
}

bool XDBaseThread::join()
{
    if (isDetach_) {
        return false;
    }
    if (NULL != pid_) {
        pthread_join(pid_, NULL);
    }
    pid_ = NULL;
    return true;
}
