#ifndef XD_INTERFACE_H
#define XD_INTERFACE_H

// 简单任务接口
class XDITask
{
public:
    virtual void operator ()() = 0;
};

#endif // end xd_interface_h
