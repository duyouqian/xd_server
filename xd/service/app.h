#ifndef XD_APP_H
#define XD_APP_H

#include "../base/base_object.h"

#include "net_service.h"
#include <string>

// default service
// net_service
// timer_service -
// master_service -
// monitor_service -

class XDApp : public XDBaseObject
{
public:
    XDApp();
    virtual ~XDApp();
    
    //virtual bool init(int32 argc, char **argv);

    void run();
    void stop();
    
    const std::string& id() const { return id_; }
    const std::string& type() const { return type_; }
    bool isMaster() const { return isMaster_; }
    
    void setID(const std::string& id) { id_ = id; }
    void setType(const std::string& type) { type_ = type; }
    
    bool registerService(XDBaseService* s);
    bool hasService(const std::string& name) const;
    template <class T> T* findService(const std::string& name);

protected:
    std::string id_;
    std::string type_;
    bool isMaster_;
    bool isStopped_;
    
    // services
    XDNetService netService_;
};

template <class T>
FROCEINLINE T* XDApp::findService(const std::string &name)
{
    
}

#endif // end xd_app_h
