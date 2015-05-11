#include "app.h"
#include "../base/log.h"

XDApp::XDApp()
     : isMaster_(true)
     , isStopped_(true)
     , netService_(*this)
     , sessionService_(*this)
{

}

XDApp::~XDApp()
{
    stop();
}

bool XDApp::init(int32 argc, char **argv)
{
    XD_LOG_OPEN("log", 0, 3);
    startServices();
    return true;
}

void XDApp::run()
{
    isStopped_ = false;
    afterServices();
    while (!isStopped_) {
        ioService_.run();
    }
    XD_LOG_CLOSE();
}

void XDApp::stop()
{
    if (!isStopped_) {
        stopServices();
        isStopped_ = true;
        ioService_.wakeup();
    }
}

bool XDApp::registerService(XDBaseService *service)
{
    if (!service) {
        return false;
    }
    if (hasService(service->name())) {
        return false;
    }
    services_.push_back(service);
    std::pair<std::map<std::string, int32>::iterator, bool> res = serviceMap_.insert(std::make_pair(service->name(), services_.size() - 1));
    return res.second;
}

bool XDApp::hasService(const std::string &name) const
{
//    if (isMaster() && strcmp(name.c_str(), XD_MASTER_SERVICE_NAME) == 0) {
//        return false;
//    }
    return serviceMap_.find(name) != serviceMap_.end();
}

void XDApp::startServices()
{
    for (std::vector<XDBaseService*>::iterator it = services_.begin(); it != services_.end(); ++it) {
        (*it)->start();
    }
}

void XDApp::afterServices()
{
    for (std::vector<XDBaseService*>::iterator it = services_.begin(); it != services_.end(); ++it) {
        (*it)->afterStart();
    }
}

void XDApp::stopServices()
{
    for (std::vector<XDBaseService*>::iterator it = services_.begin(); it != services_.end(); ++it) {
        (*it)->stop();
    }
}
