#include "log.h"
#include "net_service.h"
#include "app.h"

int32 main(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);

    XDApp app;
    XDNetService net(app);
    net.start();
    XD_LOG_CLOSE();
    return 0;
}
