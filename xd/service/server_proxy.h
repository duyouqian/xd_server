#ifndef XD_SERVER_PROXY_H
#define XD_SERVER_PROXY_H

#include "../net/socket_handler.h"

#include <string>
#include <map>

class XDApp;
class XDMessage;
class XDRequest;
class XDResponse;

class XDServerProxy : public XDTcpClientSocketEventHandler
{
public:
    XDServerProxy(XDApp &app, XDSocketPtr socket, const char *id, const char *type, const char *host, int32 port);
    virtual ~XDServerProxy();
    
    XDSocketPtr socket() { return socket_; }
    const std::string& id() const { return id_; }
    const std::string& type() const { return type_; }
    const std::string& host() const { return host_; }
    const int32 port() const { return port_; }
    
    bool send(XDMessage &message);
    bool send(XDRequest &request);
    bool send(XDResponse &response);
    
    bool sendAddServerNotify(const XDServerProxies &proies);
    bool sendRemoveServerNotify(const XDServerProxy &proxy);
    
    virtual void onConnect(XDSocketPtr socket) { }
    virtual void onReConnect(XDSocketPtr socket) { }
    virtual void onDisconnect(XDSocketPtr socket) { }
    virtual void onMessage(XDSocketPtr socket, XDMessage &message);
    
protected:
    bool makeSureConnectionEstablished();
    
protected:
    XDApp &app_;
    XDSocketPtr socket_;
    std::string id_;
    std::string type_;
    std::string host_;
    int32 port_;
};

class XDServerProxyHolder : public XDBaseObject
{
public:
    XDServerProxyHolder();
    virtual ~XDServerProxyHolder();

    bool addServer(XDServerProxy *server);
    XDServerProxy* removeServer(const std::string &id);
    XDServerProxy* removeServer(XDSocketPtr socket);
    XDServerProxy* findServer(const std::string &id);
    XDServerProxy* findServer(XDSocketPtr socket);
    XDServerProxies& findServers(const std::string &type);
    XDServerProxies& enumServers();

protected:
    void removeFromProxyArr(XDServerProxy &proxy);
    void removeFromProxyMap(XDServerProxy &proxy);
    void removeFromProxyTypeMap(XDServerProxy &proxy);

protected:
    typedef std::map<std::string, XDServerProxy*> ServerProxyMap;
    typedef std::map<std::string, XDServerProxies> ServerProxyTypeMap;
    typedef std::map<XDHandle, XDServerProxy*> ServerProxySocketMap;

    XDServerProxies proxies_;
    ServerProxyMap proxyMap_;
    ServerProxyTypeMap proxyTypeMap_;
    ServerProxySocketMap proxySocketMap_;
};

#endif // end xd_server_proxy_h
