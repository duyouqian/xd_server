#include "base_object.h"
#include "atomic_counter.h"

static XDAtomicCounter counter_;

XDBaseObject::XDBaseObject()
{
    ID_ = ++counter_;
}

XDBaseObject::~XDBaseObject()
{
}
