#ifdef __linux__
#include "socket_epoll.cpp"
#endif
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined (__NetBSD__)
#include "socket_kqueue.cpp"
#endif
