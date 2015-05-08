#include "condition.h"
#include "timer.h"

XDCondition::XDCondition(XDMutex *mutex)
           : mutex_(mutex)
{
    int32 ret = pthread_cond_init(&cond_, NULL);
    isValid_ = 0 == ret;
}

XDCondition::~XDCondition()
{
    pthread_cond_destroy(&cond_);
}

int32 XDCondition::wait()
{
    int32 ret =- pthread_cond_wait(&cond_, mutex_->getMutex());
    if (0 != ret) {
        return -1;
    }
    return 0;
}

int32 XDCondition::timedwait(uint32 millisecond)
{
    if (0 == millisecond || (uint32)(-1) == millisecond) {
        // 无限wait
        return wait();
    }
    struct timespec ts;
    XDTimerUtil::getAbsTimespec(&ts, millisecond);
    int32 ret = pthread_cond_timedwait(&cond_, mutex_->getMutex(), &ts);
    if (0 != ret) {
        if (ETIMEDOUT == ret) {
            return -2;
        }
        return -1;
    }
    return 0;
}

int32 XDCondition::signal()
{
    int32 ret = pthread_cond_signal(&cond_);
    if (0 != ret) {
        return -1;
    }
    return 0;
}

int32 XDCondition::broadcast()
{
    int32 ret = pthread_cond_broadcast(&cond_);
    if (0 != ret) {
        return -1;
    }
    return 0;
}

bool XDCondition::isValid() const
{
    return isValid_;
}
