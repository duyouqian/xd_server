#include "session_service.h"
#include "session.h"
#include "../net/socket_connection.h"
#include "../base/log.h"

XDSessionID XDSessionService::sessionIDs = 0;

XDSessionService::XDSessionService(XDApp &app)
                : XDBaseService::XDBaseService(app, XD_SESSION_SERVICE_NAME)
                , factory_(NULL)
{

}

XDSessionService::~XDSessionService()
{

}

bool XDSessionService::start()
{
    if (!XDBaseService::start()) {
        return false;
    }
    return true;
}

void XDSessionService::afterStart()
{
    
}

void XDSessionService::stop()
{
    if (state_ == XDBaseService::ST_STOPPED) {
        return;
    }
    XDBaseService::stop();
}

void XDSessionService::setSessionFactory(XDSessionFactory *factory)
{
    factory_ = factory;
}

XDSessionPtr XDSessionService::createSession(XDSocketConnectionPtr socket)
{
    XDSessionPtr session;
    if (sessionSocketMaps_.find(socket->handle()) != sessionSocketMaps_.end()) {
        // 已经存在
        XD_LOG_merror("[SessionService] client:%s:%d session 已经存在", socket->address().ip, socket->address().port);
        return session;
    }
    if (factory_) {
        session = factory_->createSession(++sessionIDs, socket);
    } else {
        session = XDSessionPtr(new XDSession(++sessionIDs, socket));
    }
    if (session) {
        addSession(session);
    }
    return session;
}

XDSessionPtr XDSessionService::findSession(XDSessionID id)
{
    std::map<XDSessionID, XDSessionPtr>::iterator it = sessionIDMaps_.find(id);
    return it != sessionIDMaps_.end() ? it->second : XDSessionPtr();
}

XDSessionPtr XDSessionService::findSession(XDSocketConnectionPtr socket)
{
    std::map<XDHandle, XDSessionPtr>::iterator it = sessionSocketMaps_.find(socket->handle());
    return it != sessionSocketMaps_.end() ? it->second : XDSessionPtr();
}

void XDSessionService::destroySession(XDSessionID id)
{
    std::map<XDSessionID, XDSessionPtr>::iterator it = sessionIDMaps_.find(id);
    if (it != sessionIDMaps_.end()) {
        it->second->destroy();
        sessionSocketMaps_.erase(it->second->socket()->handle());
        sessionIDMaps_.erase(it);
    }
}

void XDSessionService::destroySession(XDSocketConnectionPtr socket)
{
    std::map<XDHandle, XDSessionPtr>::iterator it = sessionSocketMaps_.find(socket->handle());
    if (it != sessionSocketMaps_.end()) {
        it->second->destroy();
        sessionIDMaps_.erase(it->second->id());
        sessionSocketMaps_.erase(it);
    }
}

void XDSessionService::addSession(XDSessionPtr session)
{
    if (session) {
        sessionIDMaps_[session->id()] = session;
        sessionSocketMaps_[session->socket()->handle()] = session;
    }
}