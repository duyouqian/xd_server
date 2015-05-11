#include "socket_server.h"
#include "../service/net_service.h"
#include "../base/log.h"
#include <string.h>
#include <errno.h>

XDHandle XDSocketServer::sHandle_ = 0;

XDSocketServer::XDSocketServer(XDNetService &netService)
              : serverSocketFD_(this)
              , netService_(netService)
              , state_(ST_INIT)
              , isStop_(true)
              , handle_(++sHandle_)
{

}

XDSocketServer::~XDSocketServer()
{
    //stop();
}

bool XDSocketServer::start()
{
    XD_LOG_mdebug("[SocketServer] SocketServer start");
    if (state_ == ST_INITED || state_ == ST_CLOSED) {
        bool ret = false;
        do {
            XD_LOG_mdebug("[SocketServer] 开始创建 socket_acceptor");
            ret = serverSocketFD_.create();
            if (!ret) {
                XD_LOG_merror("[SocketServer] 创建socket_acceptor 失败");
                break;
            }
            XD_LOG_mdebug("[SocketServer] 开始添加socketAcceptor到poll");
            ret = netService_.socketDispather().acceptSocket(serverSocketFD_.fd(), &serverSocketFD_);
            if (!ret) {
                XD_LOG_merror("[SocketServer] 添加socketAcceptor到poll 失败");
                serverSocketFD_.close();
                break;
            }
            state_ = ST_STARTED;
            isStop_ = false;
            XD_LOG_mdebug("[SocketServer] SocketServer start succ 监听地址:%s:%d", serverSocketFD_.address().ip, serverSocketFD_.address().port);
            return true;
        } while (0);
    }
    XD_LOG_mdebug("[SocketServer] SocketServer start failure");
    return false;
}

bool XDSocketServer::stop()
{
    if (state_ != ST_STARTED) {
        return false;
    }
    isStop_ = true;
    state_ = ST_CLOSED;
    netService_.socketDispather().delSocket(serverSocketFD_.fd(), serverSocketFD_.workThreadIndex());
    serverSocketFD_.close();
    return true;
}
