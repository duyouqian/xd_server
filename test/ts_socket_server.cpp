#include "log.h"
#include "socket_util.h"
#include "socket_poll.h"

int32 main(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);

    XDSockFD serverFD = XDSocketUtil::createServerSocket("0.0.0.0", 5000, XD_SOCKETUTIL_BACKLOG);
    XDPollFD pfd;
    if (XDSocketUtil::isInvalid(serverFD)) {
        XD_LOG_merror("[Server] 创建server socket 失败");
        goto err;
    }
    pfd = XDSocketPoll::create();
    if (XDSocketPoll::isInvalid(pfd)) {
        XDSocketUtil::close(serverFD);
        XD_LOG_merror("[Server] 创建poll 失败");
        goto err;
    }
    XDSocketPoll::add(pfd, serverFD, &serverFD);
    XDSocketUtil::XDSockAddr clientAddr;
    XDSocketPoll::XDEvent event[1024];
    int32 n;
    int32 i;
    while (1) {
        n = XDSocketPoll::wait(pfd, event, 1024);
        if (n < 1) {
            continue;
        }
        for (i = 0; i < n; ++i) {
            if (event[i].ptr == &serverFD) {
                XDSockFD connFD = XDSocketUtil::accept(serverFD, &clientAddr);
                if (XDSocketUtil::isInvalid(connFD)) {
                    continue;
                }
                XD_LOG_mdebug("[Server] 新连接FD:%d addr:%s:%d", connFD, clientAddr.ip, clientAddr.port);
                XDSocketPoll::add(pfd, serverFD, NULL);
            } else {
                if (event[i].read) {
                    //readN = XDSocketUtil::read()
                }
                if (event[i].write) {

                }
            }
        }
    }
err:
    XD_LOG_CLOSE();
    return 0;
}
