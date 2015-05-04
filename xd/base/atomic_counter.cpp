#include "atomic_counter.h"

XDAtomicCounter::XDAtomicCounter()
               : counter_(0)
{
    
}

XDAtomicCounter::XDAtomicCounter(int32 value)
               : counter_(value)
{
    
}

XDAtomicCounter::XDAtomicCounter(const XDAtomicCounter &counter)
               : counter_(counter.value())
{
    
}

XDAtomicCounter::~XDAtomicCounter()
{
    
}

XDAtomicCounter& XDAtomicCounter::operator=(const XDAtomicCounter &counter)
{
    __sync_lock_test_and_set(&counter_, counter.value());
    return *this;
}

XDAtomicCounter& XDAtomicCounter::operator=(int32 value)
{
    __sync_lock_test_and_set(&counter_, value);
    return *this;
}