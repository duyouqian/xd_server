#include "rpc_message.h"

XDRpcRequest::XDRpcRequest(uint32 serverID,
                           uint32 methodID,
                           uint32 cmdID,
                           uint32 bodySize,
                           XDReplyMessageHandler *handler,
                           int32 timeout)
           : message_(cmdID, (handler ? XDMESSAGE_FLAG_HASRELPID : 0) | XDMESSAGE_FLAG_HASSERVERID | XDMESSAGE_FLAG_HASMETHODID, bodySize)
           , serverID_(serverID)
           , methidID_(methodID)
           , handler_(handler)
           , timeout_(timeout)
{
    
}

XDRpcResponse::XDRpcResponse(uint64 replyID,
                       uint32 bodySize)
             : XDResponse::XDResponse(replyID, bodySize)
{
    
}