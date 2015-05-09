#include "session.h"
#include "../net/socket_connection.h"

XDSession::XDSession(XDSessionID id, XDSocketConnectionPtr socket)
         : id_(id)
         , socket_(socket)
{

}

XDSession::~XDSession()
{

}

void XDSession::send(XDMessage &message)
{
    socket_->send(message);
}

void XDSession::destroy()
{
    socket_->onClose();
}
