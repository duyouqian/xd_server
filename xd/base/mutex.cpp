#include "mutex.h"
#include "timer.h"
#include <errno.h>

XDMutex::XDMutex()
{
    int32 ret;
    pthread_mutexattr_t attr;
    ret = pthread_mutexattr_init(&attr);
    if (0 != ret) {
        // 设置互斥量属性失败
        ret = pthread_mutex_init(&mutex_, NULL);
        isValid_ = false;
    } else {
        // 开启互斥量错误检查
#ifdef __unix__
        ret = pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK_NP);
#endif
        ret = pthread_mutex_init(&mutex_, &attr);
        pthread_mutexattr_destroy(&attr);
        isValid_ = true;
    }
}

XDMutex::~XDMutex()
{
    pthread_mutex_destroy(&mutex_);
}

int32 XDMutex::lock()
{
    int32 ret = pthread_mutex_lock(&mutex_);
    if (0 != ret) {
        // 加锁失败
        return -1;
    }
    return 0;
}

int32 XDMutex::timedlock(uint32 millisecond)
{
#ifdef __unix__
    if (0 == millisecond || (uint32)(-1) == millisecond) {
        // 无限lock
        return lock();
    }
    struct timespec ts;
    XDTimerUtil::getAbsTimespec(&ts, millisecond);
    int32 ret = pthread_mutex_timedlock(&mutex_, &ts);
    if (0 != ret) {
        if (ETIMEDOUT == ret) {
            return -2;
        }
        return -1;
    }
    return 0;
#endif
    return lock();
}

int32 XDMutex::unlock()
{
    int32 ret = pthread_mutex_unlock(&mutex_);
    if (0 != ret) {
        // 加锁失败
        return -1;
    }
    return 0;
}

pthread_mutex_t* XDMutex::getMutex()
{
    return &mutex_;
}

bool XDMutex::isValid() const
{
    return isValid_;
}
