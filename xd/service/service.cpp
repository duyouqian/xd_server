#include "service.h"
#include "app.h"

XDBaseService::XDBaseService(XDApp &app, std::string name)
             : app_(app)
             , name_(name)
{
    app.registerService(this);
}

XDBaseService::~XDBaseService()
{

}

bool XDBaseService::start()
{
    if (ST_STARTED == state_) {
        return false;
    }

    state_ = ST_STARTED;
    return true;
}

void XDBaseService::stop()
{
    state_ = ST_STOPPED;
}
