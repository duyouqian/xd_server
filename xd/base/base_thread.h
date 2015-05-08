#ifndef XD_BASE_THREAD_H
#define XD_BASE_THRRAD_H

#include "base_object.h"
#include <pthread.h>
#include <string>

class XDBaseThread : public XDBaseObject
{
public:
    XDBaseThread();
    XDBaseThread(const char *name);
    virtual ~XDBaseThread();
    virtual bool beforeRun() { return false; }
    virtual bool run() = 0;
    virtual bool runAfter() { return false; }

    virtual bool start();
    virtual bool join();
    virtual bool detach();

    std::string& name() { return name_; }

public:
    static uint32 getCurrentThreadID();
protected:
    pthread_t pid_;
    bool isDetach_;
    std::string name_;
};

#endif // end xd_base_thread_h
