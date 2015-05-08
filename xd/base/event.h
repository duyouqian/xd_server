#ifndef XD_EVENT_H
#define XD_EVENT_H

#include "base_object.h"
#include "mutex.h"
#include "condition.h"

class XDSyncEvent : public XDBaseObject
{
public:
    XDSyncEvent();
    XDSyncEvent(bool autoReset);
    ~XDSyncEvent();

    void set();
    void wait();
    void wait(int32 millisecond);
    void reset();

protected:
    bool isAutoReset_;
    volatile bool state_;
    XDMutex mutex_;
    XDCondition cond_;
};

#endif // end xd_event_h
