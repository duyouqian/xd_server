#ifndef XD_SESSION_H
#define XD_SESSION_H

#include "../base/base_object.h"

class XDMessage;

// a client connection on server
class XDSession : public XDBaseObject
{
public:
    XDSession(XDSessionID id, XDSocketConnectionPtr socket);
    virtual ~XDSession();
    
    virtual void send(XDMessage &message);
    virtual void destroy();
    
    XDSessionID id() const { return id_; }
    XDSocketConnectionPtr socket() { return socket_; }
    
protected:
    XDSessionID id_;
    XDSocketConnectionPtr socket_;
};

#endif // end xd_session_h