#include "log.h"
#include "socket_util.h"
#include "thread.h"
#include "runnable.h"
#include "rpc_message.h"
#include "net_service.h"
#include "app.h"
#include "socket_connecter.h"

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
//    XDApp app;
//    XDNetService net(app);
//    net.frontedListen(5000);
//    net.start();
   // net.rpcConnect("127.0.0.1", 8000, NULL);
    
    //XDSocketConnecter conn;
    //conn.connect("127.0.01", 5000, NULL);

    XDSockFD connFD = XDSocketUtil::connect("127.0.0.1", 5000);
    XDRpcRequest req(1, 1, 1, 0, NULL, -1);
    XDMessage &msg = (XDMessage&)req;
    XDSocketUtil::write(connFD, msg.data(), msg.size());
    //XDSocketUtil::close(connFD);
    sleep(100);
    XD_LOG_CLOSE();
    return 0;
}
