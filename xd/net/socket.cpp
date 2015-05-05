#include "socket.h"

XDBaseSocket::XDBaseSocket()
            : fd_(-1)
            , state_(ST_INIT)
{

}

XDBaseSocket::~XDBaseSocket()
{
    close();
}

void XDBaseSocket::close()
{
    if (state_ == ST_CONNECTED || state_ == ST_HALFCLOSE) {
        XDSocketUtil::close(fd_);
        fd_ = -1;
        state_ = ST_CLOSED;
    }
}

int32 XDBaseSocket::send(void *data, int32 len)
{
    if (state_ != ST_CONNECTED) {
        return -1;
    }
    return XDSocketUtil::write(fd_, data, len);
}

int32 XDBaseSocket::read(void *data, int32 len)
{
    if (state_ != ST_CONNECTED) {
        return -1;
    }
    return XDSocketUtil::read(fd_, data, len);
}
