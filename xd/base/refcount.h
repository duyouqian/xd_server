#ifndef XD_REF_COUNTED_H
#define XD_REF_COUNTED_H

#include "base_object.h"
#include "atomic_counter.h"

class XDRefCounted : public XDBaseObject
{
public:
    XDRefCounted();
    virtual ~XDRefCounted();
    void addRef();
    void release();
    int32 referenceCount() const;

protected:
    XDAtomicCounter counter_;
};

FROCEINLINE void XDRefCounted::addRef()
{
    ++counter_;
}

FROCEINLINE void XDRefCounted::release()
{
    if (--counter_ == 0) {
        delete this;
    }
}

FROCEINLINE int32 XDRefCounted::referenceCount() const
{
    return counter_.value();
}

#endif // end xd_ref_counted_h
