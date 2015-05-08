#ifndef XD_MUTEX_H
#define XD_MUTEX_H

#include "base_object.h"
#include <pthread.h>

class XDMutex : public XDBaseObject
{
public:
    XDMutex();
    ~XDMutex();
    int32 lock();
    int32 timedlock(uint32 millisecond);
    int32 unlock();
    bool isValid() const;
    pthread_mutex_t* getMutex();
private:
    pthread_mutex_t mutex_;
    bool isValid_;
};

#endif // end xd_mutex_h
