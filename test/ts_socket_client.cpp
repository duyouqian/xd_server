#include "log.h"
#include "socket_util.h"

int32 main(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);

    XDSockFD connFD = XDSocketUtil::connect("127.0.0.1", 5000);
    XDSocketUtil::close(connFD);

    XD_LOG_CLOSE();
    return 0;
}
