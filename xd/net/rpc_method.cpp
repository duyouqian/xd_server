#include "rpc_method.h"

XDRpcMethod::XDRpcMethod()
{
    
}

XDRpcMethod::~XDRpcMethod()
{
    ServerMap::iterator it = serverMethodMaps_.begin();
    for (; it != serverMethodMaps_.end(); ++it) {
        delete it->second;
    }
    serverMethodMaps_.clear();
}

void XDRpcMethod::registerMethod(int32 serverID, int32 methodID, void (*method)(XDMessage &message))
{
    ServerMap::iterator it = serverMethodMaps_.find(serverID);
    MethodMap *maps;
    if (it == serverMethodMaps_.end()) {
        maps = new MethodMap();
        serverMethodMaps_[serverID] = maps;
    } else {
        maps = it->second;
    }
    MethodMap::iterator methodIt = maps->find(methodID);
    if (methodIt != maps->end()) {
        return;
    }
    XDRpcHandlerInfo handlerInfo = {methodID, method};
    maps->insert(std::make_pair(methodID, handlerInfo));
    //maps[methodID] = handlerInfo;
}

void XDRpcMethod::processMethod(int32 serverID, int32 methodID, XDMessage &message)
{
    ServerMap::iterator it = serverMethodMaps_.find(serverID);
    MethodMap *maps;
    if (it != serverMethodMaps_.end()) {
        maps = it->second;
        XDRpcHandlerInfo handlerInfo;
        MethodMap::iterator methodIt = maps->find(methodID);
        if (methodIt != maps->end()) {
            handlerInfo = methodIt->second;
            handlerInfo.handler(message);
        }
    }
}