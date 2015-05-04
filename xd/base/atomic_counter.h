#ifndef XD_ATOMIC_COUNTER_H
#define XD_ATOMIC_COUNTER_H

#include "types.h"

class XDAtomicCounter
{
public:
    XDAtomicCounter();
    explicit XDAtomicCounter(int32 value);
    XDAtomicCounter(const XDAtomicCounter &counter);
    ~XDAtomicCounter();
    XDAtomicCounter& operator = (const XDAtomicCounter &counter);
    XDAtomicCounter& operator = (int32 value);
    operator int32 () const;
    int32 value() const;
    int32 operator ++ (); // ++counter;
    int32 operator ++ (int32); // counter++;
    int32 operator -- (); // --counter;
    int32 operator -- (int32); // counter--;
    bool operator ! () const;
    
private:
    int32 counter_;
};

FROCEINLINE XDAtomicCounter::operator int32 () const
{
    return counter_;
}

FROCEINLINE int32 XDAtomicCounter::value() const
{
    return counter_;
}

FROCEINLINE int32 XDAtomicCounter::operator ++ ()
{
    return __sync_add_and_fetch(&counter_, 1);
}

FROCEINLINE int32 XDAtomicCounter::operator ++ (int32)
{
    return __sync_fetch_and_add(&counter_, 1);
}

FROCEINLINE int32 XDAtomicCounter::operator -- ()
{
    return __sync_sub_and_fetch(&counter_, 1);
}

FROCEINLINE int32 XDAtomicCounter::operator -- (int32)
{
    return __sync_fetch_and_sub(&counter_, 1);
}

FROCEINLINE bool XDAtomicCounter::operator ! () const
{
    return counter_ == 0;
}

#endif // end xd_atomic_counter_h