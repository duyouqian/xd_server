#include "log.h"
#include "socket_util.h"
#include "thread.h"
#include "runnable.h"

class NetThread : public XDRunnable
{
public:
    NetThread(bool *isStop);
    ~NetThread();
    
    bool run();
    void send(const void *data, int32 len);
    
private:
    XDThread thread_;
    bool *isStop_;
    XDSockFD fd_;
};

int32 main(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);

    XDSockFD connFD = XDSocketUtil::connect("127.0.0.1", 5000);
    XDSocketUtil::close(connFD);

    XD_LOG_CLOSE();
    return 0;
}
