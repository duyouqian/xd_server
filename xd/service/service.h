#ifndef XD_SERVICE_H
#define XD_SERVICE_H

#include "../base/base_object.h"

#include <string>

class XDApp;

class XDBaseService : public XDBaseObject
{
public:
    enum State
    {
        ST_STOPPED,
        ST_STARTED,
    };
public:
    explicit XDBaseService(XDApp &app, std::string name);
    virtual ~XDBaseService();

    virtual bool start();
    virtual void afterStart() = 0;
    virtual void stop() = 0;

    XDApp& app() { return app_; }
    const std::string& name() const { return name_; }

protected:
    XDApp &app_;
    std::string name_;
    State state_;
};

#endif // end xd_service_h
