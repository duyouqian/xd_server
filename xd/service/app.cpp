#include "app.h"

XDApp::XDApp()
     : netService_(*this)
     , isStopped_(true)
{

}

XDApp::~XDApp()
{

}

void XDApp::run()
{
    isStopped_ = false;
    while (!isStopped_) {
        ioService_.run();
    }
}

void XDApp::stop()
{
    isStopped_ = true;
    ioService_.wakeup();
}
