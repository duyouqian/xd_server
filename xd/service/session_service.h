#ifndef XD_SESSION_SERVICE_H
#define XD_SESSION_SERVICE_H

#include "service.h"
#include "../base/types.h"
#include <map>

#define XD_SESSION_SERVICE_NAME "SessionService"

class XDSessionFactory
{
public:
    virtual XDSessionPtr createSession(XDSessionID id, XDSocketConnectionPtr socket) = 0;
};

class XDSessionService : public XDBaseService
{
public:
    XDSessionService(XDApp &app);
    ~XDSessionService();

    virtual bool start();
    virtual void afterStart();
    virtual void stop();

    void setSessionFactory(XDSessionFactory *factory);
    
    XDSessionPtr createSession(XDSocketConnectionPtr socket);
    XDSessionPtr findSession(XDSessionID id);
    XDSessionPtr findSession(XDSocketConnectionPtr socket);
    
    void destroySession(XDSessionID id);
    void destroySession(XDSocketConnectionPtr socket);
    
private:
    void addSession(XDSessionPtr session);

private:
    XDSessionFactory *factory_;
    std::map<XDSessionID, XDSessionPtr> sessionIDMaps_;
    std::map<XDHandle, XDSessionPtr> sessionSocketMaps_;
    static XDSessionID sessionIDs;
};

#endif // end xd_session_service_h
