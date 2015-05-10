#include "socket_poll.h"
#include "../base/timer.h"

#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/event.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

XDPollFD XDSocketPoll::create()
{
    return kqueue();
}

void XDSocketPoll::release(XDPollFD fd)
{
    ::close(fd);
}

int32 XDSocketPoll::add(XDPollFD fd, XDSockFD sock, void *ud)
{
    struct kevent ke;
    EV_SET(&ke, sock, EVFILT_READ, EV_ADD, 0, 0, ud);
    if (kevent(fd, &ke, 1, NULL, 0, NULL) == -1) {
        return 1;
    }
    EV_SET(&ke, sock, EVFILT_WRITE, EV_ADD, 0, 0, ud);
    if (kevent(fd, &ke, 1, NULL, 0, NULL) == -1) {
        EV_SET(&ke, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
        kevent(fd, &ke, 1, NULL, 0, NULL);
        return 1;
    }
    EV_SET(&ke, sock, EVFILT_WRITE, EV_DISABLE, 0, 0, ud);
    if (kevent(fd, &ke, 1, NULL, 0, NULL) == -1) {
        del(fd, sock);
        return 1;
    }
    return 0;
}

void XDSocketPoll::del(XDPollFD fd, XDSockFD sock)
{
    struct kevent ke;
    EV_SET(&ke, sock, EVFILT_READ, EV_DELETE, 0, 0, NULL);
    kevent(fd, &ke, 1, NULL, 0, NULL);
    EV_SET(&ke, sock, EVFILT_WRITE, EV_DELETE, 0, 0, NULL);
    kevent(fd, &ke, 1, NULL, 0, NULL);
}

void XDSocketPoll::write(XDPollFD fd, XDSockFD sock, void *ud, bool enable)
{
    struct kevent ke;
    EV_SET(&ke, sock, EVFILT_WRITE, enable ? EV_ENABLE : EV_DISABLE, 0, 0, ud);
    if (kevent(fd, &ke, 1, NULL, 0, NULL) == -1) {
        // todo: check error
    }
}

int32 XDSocketPoll::wait(XDPollFD fd, XDEvent *e, int32 max)
{
    struct kevent ev[max];
    int n = kevent(fd, NULL, 0, ev, max, NULL);

    int i;
    for (i = 0; i < n; ++i) {
        e[i].ptr = ev[i].udata;
        unsigned filter = ev[i].filter;
        e[i].write = (filter == EVFILT_WRITE);
        e[i].read = (filter == EVFILT_READ);
    }

    return n;
}

int32 XDSocketPoll::wait(XDPollFD fd, XDEvent *e, int32 max, int32 millisecond)
{
    struct kevent ev[max];
    struct timespec ts;
    XDTimerUtil::getAbsTimespec(&ts, millisecond);
    int n = kevent(fd, NULL, 0, ev, max, -1 == millisecond ? NULL : &ts);
    
    int i;
    for (i = 0; i < n; ++i) {
        struct kevent eeee = ev[i];
        e[i].ptr = ev[i].udata;
        unsigned filter = ev[i].filter;
        e[i].write = (filter == EVFILT_WRITE);
        e[i].read = (filter == EVFILT_READ);
    }
    
    return n;
}
