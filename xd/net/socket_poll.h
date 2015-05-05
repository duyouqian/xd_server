#ifndef XD_SOCKET_POLL_H
#define XD_SOCKET_POLL_H

#include "../base/types.h"

namespace XDSocketPoll {

struct XDEvent
{
    void *ptr;
    bool read;
    bool write;
};

// 判断poll fd is invalid
FROCEINLINE bool isInvalid(XDPollFD fd)
{
    return -1 == fd;
}

// 创建 poll
XDPollFD create();

// 释放 poll
void release(XDPollFD fd);

// 添加socket event
// return 1: success, 0 : failure
int32 add(XDPollFD fd, XDSockFD sock, void *ud);

// 删除
void del(XDPollFD fd, XDSockFD sock);

// write
void write(XDPollFD fd, XDSockFD sock, void *ud, bool enable);

// 等待
int32 wait(XDPollFD fd, XDEvent *e, int32 max);

}

#endif // end xd_socket_poll_h
