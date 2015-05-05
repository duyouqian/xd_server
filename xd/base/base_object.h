#ifndef XD_BASE_OBJECT_H
#define XD_BASE_OBJECT_H

#include "noncopyable.h"
#include "types.h"

class XDBaseObject : public XDNoncopyable
{
public:
    XDBaseObject();
    virtual ~XDBaseObject();
    int32 getID() const { return ID_; }
protected:
    int32 ID_; // 对象唯一ID
};

#endif // end xd_base_object_h
