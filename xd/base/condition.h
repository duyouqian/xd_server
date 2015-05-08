#ifndef XD_CONDITION_H
#define XD_CONDITION_H

#include "base_object.h"
#include "mutex.h"

class XDCondition : public XDBaseObject
{
public:
    explicit XDCondition(XDMutex *mutex);
    ~XDCondition();

    int32 wait();
    int32 timedwait(uint32 millisecond);
    int32 signal();
    int32 broadcast();
    bool isValid() const;
private:
    XDMutex *mutex_;
    pthread_cond_t cond_;
    bool isValid_;
};

#endif // end xd_condition_h
