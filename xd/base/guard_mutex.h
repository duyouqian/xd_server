#ifndef XD_GUARD_MUTEX_H
#define XD_GUARD_MUTEX_H

#include "base_object.h"

class XDMutex;

// 栈上互斥量
class XDGuardMutex : public XDBaseObject
{
public:
    explicit XDGuardMutex(XDMutex *mutex);
    ~XDGuardMutex();
    bool isLocked() const;
private:
    XDMutex *mutex_;
    bool isLocked_;
};

#endif
