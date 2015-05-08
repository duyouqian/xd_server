#include "socket_util.h"
#include "socket_addr.h"
#include "log.h"

#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define XD_CONNECT_NONE 0
#define XD_CONNECT_NONBLOCK 1

// socket listen
int32 socketListen(XDSockFD fd, struct sockaddr *addr, socklen_t addrLen, int32 backlog)
{
    // 先绑定后监听
    if (::bind(fd, addr, addrLen) == -1) {
        // 失败
        XD_LOG_merror("[Socket] bind(Socket):%s", strerror(errno));
        ::close(fd);
        return -1;
    }
    // 监听
    if (::listen(fd, backlog) == -1) {
        // 失败
        XD_LOG_merror("[Socket] listen(Socket):%s", strerror(errno));
        ::close(fd);
        return -1;
    }
    return 0;
}

// create generic server socket
XDSockFD createGenericServerSocket(const char *host, int32 port, int32 af, int32 backlog)
{
    XDSockFD fd;
    int32 rv;
    char port_[6];
    struct addrinfo hints, *serverInfo, *p;
    snprintf(port_, sizeof(port_), "%d", port);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = af;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if ((rv = getaddrinfo(host, port_, &hints, &serverInfo)) != 0) {
        // 失败
        XD_LOG_merror("[Socket] getServerSocketInfo:%s", gai_strerror(rv));
        return -1;
    }
    for (p = serverInfo; p != NULL; p = p->ai_next) {
        if ((fd = ::socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        // ipv6
        // ipv4
        if (XDSocketUtil::setReuseAddr(fd, true) == -1) {
            goto error;
        }
        if (socketListen(fd, p->ai_addr, p->ai_addrlen, backlog) == -1) {
            goto error;
        }
        goto end;
    }
    if (NULL == p) {
        // 失败
        XD_LOG_merror("[Socket] getServerSocketInfo: 不能绑定Socket");
        goto error;
    }
error:
    fd = -1;
end:
    freeaddrinfo(serverInfo);
    return fd;
}

// generic accept
XDSockFD genericAccept(XDSockFD fd, struct sockaddr *addr, socklen_t *addrLen)
{
    XDSockFD connFD = -1;
    while (1) {
        connFD = ::accept(fd, addr, addrLen);
        if (-1 == connFD) {
            if (errno == EINTR) {
                continue;
            } else {
                // 失败
                XD_LOG_merror("[Socket] accept:%s", strerror(errno));
                return -1;
            }
        }
        break;
    }
    return connFD;
}

// generic connect
XDSockFD genericConnect(const char *destAddr, int32 destPort, const char *sourceAddr, int32 flags)
{
    XDSockFD connFD = -1;
    int32 rv;
    char port_[6];
    struct addrinfo hints, *serverInfo, *p, *b, *bserverInfo;
    snprintf(port_, sizeof(port_), "%d", destPort);
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if ((rv = getaddrinfo(destAddr, port_, &hints, &serverInfo)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] getServerSocketInfo:%s", gai_strerror(rv));
        return -1;
    }
    for (p = serverInfo; p != NULL; p = p->ai_next) {
        if ((connFD = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            continue;
        }
        if (XDSocketUtil::setReuseAddr(connFD, true) == -1) {
            goto error;
        }
        if ((flags & XD_CONNECT_NONBLOCK) &&
            (XDSocketUtil::setSocketNonblock(connFD, true) == -1)) {
            goto error;
        }
        if (sourceAddr) {
            // 绑定地址
            int32 bound = 0;
            if ((rv = getaddrinfo(sourceAddr, NULL, &hints, &bserverInfo)) == -1) {
                // 失败
                XD_LOG_merror("[Socket] buind sourceAddr:%s getServerSocketInfo:%s", sourceAddr, gai_strerror(rv));
                goto end;
            }
            for (b = bserverInfo; b != NULL; b = b->ai_next) {
                if (::bind(connFD, b->ai_addr, b->ai_addrlen) != -1) {
                    bound = 1;
                    break;
                }
            }
            freeaddrinfo(bserverInfo);
            if (!bound) {
                // 失败
                XD_LOG_merror("[Socket] buind sourceAddr:%s getServerSocketInfo:%s", sourceAddr, strerror(errno));
                goto end;
            }
        }
        if (::connect(connFD, p->ai_addr, p->ai_addrlen) == -1) {
            if ((errno == EINPROGRESS) &&
                (flags & XD_CONNECT_NONBLOCK)) {
                goto end;
            }
            ::close(connFD);
            connFD = -1;
            continue;
        }
        goto end;
    }
error:
    if (connFD != -1) {
        ::close(connFD);
        connFD = -1;
    }
end:
    freeaddrinfo(serverInfo);
    return connFD;
}

int32 XDSocketUtil::setSocketNonblock(XDSockFD fd, bool nonBlock)
{
    int32 flags;
    if ((flags = fcntl(fd, F_GETFL)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] fcntl(F_GETFL):%s", strerror(errno));
        return -1;
    }
    if (nonBlock) {
        flags |= O_NONBLOCK;
    } else {
        flags &= ~ O_NONBLOCK;
    }
    if (fcntl(fd, F_SETFL, flags) == -1) {
        // 失败
        XD_LOG_merror("[Socket] fcntl(F_SETFL, O_NONBLOCK):%s", strerror(errno));
        return -1;
    }
    return 0;
}

int32 XDSocketUtil::setSocketTcpDelay(XDSockFD fd, bool on)
{
    int32 val = on ? 1 : 0;
    if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, &val, sizeof(val)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] setsockopt(TCP_NODELAY):%s", strerror(errno));
        return -1;
    }
    return 0;
}

int32 XDSocketUtil::setReuseAddr(XDSockFD fd, bool reuse)
{
    int32 flag = reuse ? 1 : 0;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] setsockopt(SO_REUSEADDR):%s", strerror(errno));
        return -1;
    }
    return 0;
}

int32 XDSocketUtil::setKeepAlive(XDSockFD fd, bool keep)
{
    int32 flag = keep ? 1 : 0;
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(flag)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] setsockopt(SO_KEEPALIVE):%s", strerror(errno));
        return -1;
    }
    return 0;
}

int32 XDSocketUtil::setSendTimeout(XDSockFD fd, uint64 millseconds)
{
    struct timeval tv;
    tv.tv_sec = millseconds / 1000;
    tv.tv_usec = (millseconds % 1000) * 1000;
    if (setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] setsockopt(SO_SNDTIMEO):%s", strerror(errno));
        return -1;
    }
    return 0;
}

int32 XDSocketUtil::setRecvTimeout(XDSockFD fd, uint64 millseconds)
{
    struct timeval tv;
    tv.tv_sec = millseconds / 1000;
    tv.tv_usec = (millseconds % 1000) * 1000;
    if (setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] setsockopt(SO_SNDTIMEO):%s", strerror(errno));
        return -1;
    }
    return 0;
}

int32 XDSocketUtil::setSendBufferSize(XDSockFD fd, int32 bufferSize)
{
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, &bufferSize, sizeof(bufferSize)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] setsockopt(SO_SNDBUF):%s", strerror(errno));
        return -1;
    }
    return 0;
}

int32 XDSocketUtil::setRecvBufferSize(XDSockFD fd, int32 bufferSize)
{
    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, &bufferSize, sizeof(bufferSize)) == -1) {
        // 失败
        XD_LOG_merror("[Socket] setsockopt(SO_SNDBUF):%s", strerror(errno));
        return -1;
    }
    return 0;
}

// nread 0: 关闭; -1 : 查看 errno; > 0 正常
// errno : EINTR, EAGAIN
// EINTR : 再试一次
// EAGAIN : 已经读到末尾了(非堵塞IO)
int32 XDSocketUtil::read(XDSockFD fd, void *buffer, int32 size)
{
    int32 nread = -1;
    nread = ::read(fd, buffer, size);
    return nread;
}

// nread 0: 关闭; -1 : 查看 errno; > 0 正常
// errno : EINTR, EAGAIN
// EINTR : 再试一次
// EAGAIN : 已经读到末尾了(非堵塞IO)
int32 XDSocketUtil::write(XDSockFD fd, void *buffer, int32 size)
{
    int32 nwrite;
    nwrite = ::write(fd, buffer, size);
    return nwrite;
}

int32 XDSocketUtil::getPeerAddrBySocketFD(XDSockFD fd, XDSockAddr *addr)
{
    struct sockaddr_storage sa;
    socklen_t len = sizeof(sa);
    int32 ret = getpeername(fd, (struct sockaddr*)&sa, &len);
    if (ret == -1) {
        XD_LOG_merror("[Socket] xd_socket_getpeeraddr:%s", strerror(errno));
        return -1;
    }
    if (sa.ss_family == AF_INET) {
        // ipv4
        struct sockaddr_in *s = (struct sockaddr_in *)&sa;
        if (addr) {
            inet_ntop(AF_INET, (void*)&(s->sin_addr), addr->ip, sizeof(addr->ip));
            addr->port = ntohs(s->sin_port);
        }
    } else {
        // ipv6
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&sa;
        if (addr) {
            inet_ntop(AF_INET, (void*)&(s->sin6_addr), addr->ip, sizeof(addr->ip));
            addr->port = ntohs(s->sin6_port);
        }
    }
    return 0;
}

XDSockFD XDSocketUtil::createServerSocket(const char *host, int32 port, int32 backlog)
{
    return createGenericServerSocket(host, port, AF_INET, backlog);
}

XDSockFD XDSocketUtil::accept(XDSockFD fd, XDSockAddr *addr)
{
    XDPollFD connFD;
    struct sockaddr_storage sa;
    socklen_t len = sizeof(sa);
    if ((connFD = genericAccept(fd, (struct sockaddr*)&sa, &len)) == -1) {
        return -1;
    }
    if (sa.ss_family == AF_INET) {
        // ipv4
        struct sockaddr_in *s = (struct sockaddr_in *)&sa;
        if (addr) {
            inet_ntop(AF_INET, (void*)&(s->sin_addr), addr->ip, sizeof(addr->ip));
            addr->port = ntohs(s->sin_port);
        }
    } else {
        // ipv6
        struct sockaddr_in6 *s = (struct sockaddr_in6 *)&sa;
        if (addr) {
            inet_ntop(AF_INET, (void*)&(s->sin6_addr), addr->ip, sizeof(addr->ip));
            addr->port = ntohs(s->sin6_port);
        }
    }
    return connFD;
}

XDSockFD XDSocketUtil::connect(const char *host, int32 port)
{
    return genericConnect(host, port, NULL, XD_CONNECT_NONE);
}

XDSockFD XDSocketUtil::connectWithNonblock(const char *host, int32 port)
{
    return genericConnect(host, port, NULL, XD_CONNECT_NONBLOCK);
}

XDSockFD XDSocketUtil::connectWithNonblockAndBindAddr(const char *host, int32 port, const char *sourceAddr)
{
    return genericConnect(host, port, sourceAddr, XD_CONNECT_NONBLOCK);
}
