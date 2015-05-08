#ifndef XD_SOCKET_ADDR_H
#define XD_SOCKET_ADDR_H

#include "types.h"

namespace XDSocketUtil {
// socket address
struct XDSockAddr
{
    char ip[32];
    int32 port;
};
}

#endif // end xd_socket_addr_h
