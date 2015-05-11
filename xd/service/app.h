#ifndef XD_APP_H
#define XD_APP_H

#include "../base/singleton.h"

#include "io_service.h"
#include "net_service.h"
#include "session_service.h"
#include <string>
#include <vector>
#include <map>

// default service
// net_service
// timer_service -
// master_service -
// monitor_service -

class XDApp : public XDSingleton<XDApp>
{
public:
    XDApp();
    virtual ~XDApp();
    
    bool init(int32 argc, char **argv);

    void run();
    void stop();
    
    const std::string& id() const { return id_; }
    const std::string& type() const { return type_; }
    bool isMaster() const { return isMaster_; }
    
    void setID(const std::string& id) { id_ = id; }
    void setType(const std::string& type) { type_ = type; }

    void post(XDITask *task) { ioService_.postTask(task); }
    
    bool registerService(XDBaseService* s);
    bool hasService(const std::string& name) const;
    template <class T> T* findService(const std::string& name);

protected:
    void startServices();
    void afterServices();
    void stopServices();

protected:
    std::string id_;
    std::string type_;
    bool isMaster_;
    bool isStopped_;

    // io service
    XDIOService ioService_;
    
    std::vector<XDBaseService*> services_;
    std::map<std::string, int32> serviceMap_;
    
    // services
    XDNetService netService_;
    XDSessionService sessionService_;
};

template <class T>
FROCEINLINE T* XDApp::findService(const std::string &name)
{
//    if (isMaster() && (strcmp(name.c_str(), XD_MASTER_SERVICE_NAME)) == 0) {
//        return NULL;
//    }
    std::map<std::string, int32>::iterator it = serviceMap_.find(name);
    return it != serviceMap_.end() ? static_cast<T*>(services_[it->second]) : NULL;
}

#endif // end xd_app_h
