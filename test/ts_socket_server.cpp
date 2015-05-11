#include "log.h"
#include "net_service.h"
#include "app.h"

#include <signal.h>

void tt(int a)
{
    XD_LOG_mdebug("[Main] signal:%d", a);
}

int32 main(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);
    //signal(SIGINT, tt);
    //signal(SIGTERM, tt);
    XDApp app;
    XDNetService net(app);
    net.frontedListen(5000);
    net.start();
    app.run();
    XD_LOG_CLOSE();
    return 0;
}
