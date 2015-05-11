#include "log.h"
#include "net_service.h"
#include "app.h"

#include <signal.h>
#include "unistd.h"

void tt(int a)
{
    XD_LOG_mdebug("[Main] signal:%d", a);
}

class Stop : public XDRunnable
{
public:
    Stop(XDApp &app) : app_(app) { }

    void start()
    {
        thread_.start(this);
    }

    bool run()
    {
        sleep(60);
        app_.stop();
        return true;
    }

private:
    XDApp &app_;
    XDThread thread_;
};

int32 main(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);
    //signal(SIGINT, tt);
    //signal(SIGTERM, tt);
    XDApp app;
    Stop t(app);
    XDNetService net(app);
    net.frontedListen(5000);
    net.start();
    t.start();
    app.run();
    XD_LOG_CLOSE();
    return 0;
}
