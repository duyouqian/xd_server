#ifndef XD_SOCKET_UTIL_H
#define XD_SOCKET_UTIL_H

#include "types.h"

namespace XDSocketUtil {
// 主机字节序转网络字节序
FROCEINLINE uint64 hostToNetwork64(uint64 value)
{
    return __builtin_bswap64(value);
    //return htobe64(value);
}
    
FROCEINLINE uint32 hostToNetwork32(uint32 value)
{
    return __builtin_bswap32(value);
    //return htobe32(value);
}
    
FROCEINLINE uint16 hostToNetwork16(uint16 value)
{
    return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
    //return __builtin_bswap16(value);
    //return htobe16(value);
}

// 网络字节序转主机字节序
FROCEINLINE uint64 networkToHost64(uint64 value)
{
    return __builtin_bswap64(value);
    //return be64toh(value);
}
    
FROCEINLINE uint32 networkToHost32(uint32 value)
{
    return __builtin_bswap32(value);
    //return be32toh(value);
}
    
FROCEINLINE uint16 networkToHost16(uint16 value)
{
    return ((value >> 8) & 0x00FF) | ((value << 8) & 0xFF00);
    //return be16toh(value);
}
    
}

#endif // end xd_socket_util_h