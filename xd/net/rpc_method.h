#ifndef XD_RPC_METHOD_H
#define XD_RPC_METHOD_H

#include "../base/types.h"
#include "../base/singleton.h"

#include <map>

class XDMessage;

struct XDRpcHandlerInfo
{
    int32 methodID; // method id
    void (*handler)(XDMessage &message); // exec rpc request
};

class XDRpcMethod : public XDSingleton<XDRpcMethod>
{
public:
    XDRpcMethod();
    ~XDRpcMethod();
    
    void registerMethod(int32 serverID, int32 methodID, void (*method)(XDMessage &message));
    void processMethod(int32 serverID, int32 methodID, XDMessage &message);
    
private:
    typedef std::map<int32, XDRpcHandlerInfo> MethodMap;
    typedef std::map<int32, MethodMap*> ServerMap;
    ServerMap serverMethodMaps_;
};


#endif // endXD_RPC_METHID_H