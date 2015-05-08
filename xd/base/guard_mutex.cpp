#include "guard_mutex.h"
#include "mutex.h"

XDGuardMutex::XDGuardMutex(XDMutex *mutex)
            : mutex_(mutex)
            , isLocked_(false)
{
    if (NULL != mutex_) {
        if (0 == mutex_->lock()) {
            isLocked_ = true;
        }
    }
}

XDGuardMutex::~XDGuardMutex()
{
    if (NULL != mutex_) {
        if (0 == mutex_->unlock()) {
            isLocked_ = false;
        }
    }
}

bool XDGuardMutex::isLocked() const
{
    return isLocked_;
}
