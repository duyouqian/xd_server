#include "app.h"

XDApp::XDApp()
     : netService_(*this)
{

}

XDApp::~XDApp()
{

}

void XDApp::run()
{
    while (!isStopped_) {
        ioService_.run();
    }
}

void XDApp::stop()
{
    isStopped_ = true;
    ioService_.wakeup();
}
