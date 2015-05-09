#ifndef XD_SESSION_SERVICE_H
#define XD_SESSION_SERVICE_H

#include "service.h"

#define XD_SESSION_SERVICE_NAME "SessionService"

class XDSessionService : public XDBaseService
{
public:
    XDSessionService(XDApp &app);
    ~XDSessionService();

    virtual bool start();
    virtual void afterStart();
    virtual void stop();



private:
};

#endif // end xd_session_service_h
