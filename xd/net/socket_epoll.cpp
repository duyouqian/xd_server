#include "socket_poll.h"

#include <netdb.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>

XDPollFD XDSocketPoll::create()
{
    return epoll_create(1024);
}

void XDSocketPoll::release(XDPollFD fd)
{
    ::close(fd);
}

int32 XDSocketPoll::add(XDPollFD fd, XDSockFD sock, void *ud)
{
    struct epoll_event ev;
    ev.events = EPOLLIN;
    ev.data.ptr = ud;
    if (epoll_ctl(fd, EPOLL_CTL_ADD, sock, &ev) == -1) {
        return 1;
    }
    return 0;
}

void XDSocketPoll::del(XDPollFD fd, XDSockFD sock)
{
    epoll_ctl(fd, EPOLL_CTL_DEL, sock , NULL);
}

void XDSocketPoll::write(XDPollFD fd, XDSockFD sock, void *ud, bool enable)
{
    struct epoll_event ev;
    ev.events = EPOLLIN | (enable ? EPOLLOUT : 0);
    ev.data.ptr = ud;
    epoll_ctl(fd, EPOLL_CTL_MOD, sock, &ev);
}

int32 XDSocketPoll::wait(XDPollFD fd, XDEvent *e, int32 max)
{
    struct epoll_event ev[max];
    int n = epoll_wait(fd , ev, max, -1);
    int i;
    for (i = 0; i < n; ++i) {
        e[i].ptr = ev[i].data.ptr;
        uint32 flag = ev[i].events;
        e[i].write = (flag & EPOLLOUT) != 0;
        e[i].read = (flag & EPOLLIN) != 0;
    }

    return n;
}
