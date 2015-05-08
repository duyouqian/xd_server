#ifndef XD_APP_H
#define XD_APP_H

#include "../base/base_object.h"

#include "net_service.h"

class XDApp : public XDBaseObject
{
public:
    XDApp();
    ~XDApp();

    void run();
    void stop();

protected:
    XDNetService netService_;
};

#endif // end xd_app_h
