#include "event.h"
#include "guard_mutex.h"

XDSyncEvent::XDSyncEvent()
           : isAutoReset_(false)
           , state_(false)
           , mutex_()
           , cond_(&mutex_)
{

}

XDSyncEvent::XDSyncEvent(bool autoReset)
           : isAutoReset_(autoReset)
           , state_(false)
           , mutex_()
           , cond_(&mutex_)
{

}

XDSyncEvent::~XDSyncEvent()
{

}

void XDSyncEvent::set()
{
    XDGuardMutex lock(&mutex_);
    state_ = true;
    cond_.broadcast();
}

void XDSyncEvent::wait()
{
    XDGuardMutex lock(&mutex_);
    while (!state_) {
        check(0 == cond_.wait());
    }
    if (isAutoReset_) {
        state_ = false;
    }
}

void XDSyncEvent::wait(int32 millisecond)
{
    XDGuardMutex lock(&mutex_);
    while (!state_) {
        check(0 == cond_.timedwait(millisecond));
    }
    if (isAutoReset_) {
        state_ = false;
    }
}

void XDSyncEvent::reset()
{
    XDGuardMutex lock(&mutex_);
    state_ = false;
}
