#include "server_proxy.h"
#include "app.h"
#include "../net/socket_connecter.cpp"
#include "../net/socket.h"
#include "../net/message.h"

XDServerProxy::XDServerProxy(XDApp &app, XDSocketPtr socket, const char *id, const char *type, const char *host, int32 port)
             : app_(app)
             , socket_(socket)
             , id_(id)
             , type_(type)
             , host_(host)
             , port_(port)
{
    
}

XDServerProxy::~XDServerProxy()
{
    if (socket_) {
        XDSocketConnecterPtr c = std::static_pointer_cast<XDSocketConnecter>(socket_);
        c->setEventHandler(NULL);
    }
}

bool XDServerProxy::send(XDMessage &message)
{
    if (makeSureConnectionEstablished()) {
        return  socket_->send(message);
    }
    return false;
}

bool XDServerProxy::send(XDRequest &request)
{
    if (makeSureConnectionEstablished()) {
        return  socket_->send(request);
    }
    return false;
}

bool XDServerProxy::send(XDResponse &response)
{
    if (makeSureConnectionEstablished()) {
        return  socket_->send(response);
    }
    return false;
}

bool XDServerProxy::sendAddServerNotify(const XDServerProxies &proies)
{
    // TODO protobuf
    return false;
}

bool XDServerProxy::sendRemoveServerNotify(const XDServerProxy &proxy)
{
   // TODO protobuf
    return false;
}

bool XDServerProxy::makeSureConnectionEstablished()
{
    if (app_.isMaster()) {
        if (socket_ && socket_->state() == XDBaseSocket::ST_CONNECTED) {
            return true;
        }
        return false;
    }
    if (!socket_) {
        socket_ = app_.connect(host_.c_str(), port_, this, false);
    } else if (socket_->state() != XDBaseSocket::ST_CONNECTED) {
        XDSocketConnecterPtr c = std::static_pointer_cast<XDSocketConnecter>(socket_);
        c->connect(host_.c_str(), port_, this, false);
    }
    if (socket_ && socket_->state() == XDBaseSocket::ST_CONNECTED) {
        return true;
    }
    return false;
}