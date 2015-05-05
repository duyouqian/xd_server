#ifndef XD_SOCKET_UTIL_H
#define XD_SOCKET_UTIL_H

#include <unistd.h>
#include "types.h"

#define XD_SOCKETUTIL_BACKLOG 32

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

// socket address
struct XDSockAddr
{
    char ip[32];
    int32 port;
};

// 设置sock为堵塞模式, on : true 为非堵塞, false : 为堵塞
// return 0 : success, -1 : failure
int32 setSocketNonblock(XDSockFD fd, bool on);

// 设置Nagle算法, on : true 开启, false : 关闭
// return 0 : success, -1 : failure
int32 setSocketTcpDelay(XDSockFD fd, bool on);

// 设置地址重用, on : true 重用, false : 不重用
// return 0 : success, -1 : failure
int32 setReuseAddr(XDSockFD fd, bool on);

// 设置保活, on : true 保活, false : 不保活
// return 0 : success, -1 : failure
int32 setKeepAlive(XDSockFD fd, bool on);

// 设置发送超时
// return 0 : success, -1 : failure
int32 setSendTimeout(XDSockFD fd, uint64 millseconds);

// 设置接受超时
// return 0 : success, -1 : failure
int32 setRecvTimeout(XDSockFD fd, uint64 miallseconds);

// 设置发送BUFFSIZE
// return 0 : success, -1 : failure
int32 setSendBufferSize(XDSockFD fd, int32 bufferSize);

// 设置接受BUFFSIZE
// return 0 : success, -1 : failure
int32 setRecvBufferSize(XDSockFD fd, int32 bufferSize);

// 读取socket缓冲区数据
// return 读取的字节数(-1 : 错误, 0 : 对端关闭)
int32 read(XDSockFD fd, void *buffer, int32 size);

// 发送数据到发送缓冲区
// return 发送的字节数
int32 write(XDSockFD fd, void *buffer, int32 size);

// 获取远端地址
// return return 0 : success, -1 : failure
int32 getPeerAddrBySocketFD(XDSockFD fd, XDSockAddr *addr);

// 关闭sock连接
FROCEINLINE void close(XDSockFD fd)
{
    ::close(fd);
}

// 判断是否无效
// return true : 无效, flase : 有效
FROCEINLINE bool isInvalid(XDSockFD fd)
{
    return -1 == fd;
}


// server socket

// 创建server socket
XDSockFD createServerSocket(const char *host, int32 port, int32 backlog);

// 接受socket
XDSockFD accept(XDSockFD fd, XDSockAddr *addr);

// client socket

// 连接ServerSocket
XDSockFD connect(const char *host, int32 port);

// 非堵塞连接
XDSockFD connectWithNonblock(const char *host, int32 port);

// 非堵塞连接并且绑定地址
XDSockFD connectWithNonblockAndBindAddr(const char *host, int32 port, const char *sourceAddr);

}

#endif // end xd_socket_util_h
