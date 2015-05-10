SET(XDBASEDIR .)
FILE(GLOB_RECURSE ALL_SRC_BASE_XD ${XDBASEDIR}/base/*.h ${XDBASEDIR}/base/*.cpp)
FILE(GLOB_RECURSE ALL_SRC_NET_XD ${XDBASEDIR}/net/*.h ${XDBASEDIR}/net/*.cpp)
FILE(GLOB_RECURSE ALL_SRC_SERVICE_XD ${XDBASEDIR}/service/*.h ${XDBASEDIR}/service/*.cpp)
SOURCE_GROUP("base" FILES ${ALL_SRC_BASE_XD})
SOURCE_GROUP("net" FILES ${ALL_SRC_NET_XD})
SOURCE_GROUP("service" FILES ${ALL_SRC_SERVICE_XD})

IF(OS_APPLE)
    SET(SRC_BASE_XD ${ALL_SRC_BASE_XD})
    SET(SRC_NET_XD net/socket_poll.h
                   net/socket_poll.cpp
                   net/socket_kqueue.cpp
                   net/socket.h
                   net/socket.cpp
                   net/socket_acceptor.h
                   net/socket_acceptor.cpp
                   net/socket_connection.h
                   net/socket_connection.cpp
                   net/socket_connecter.h
                   net/socket_connecter.cpp
                   net/socket_server.h
                   net/socket_server.cpp
                   net/socket_server_dispatcher.h
                   net/socket_server_dispatcher.cpp
                   net/socket_handler.h
                   net/message.h
                   net/message.cpp
                   net/rpc_message.h
                   net/rpc_message.cpp
                   net/rpc_method.h
                   net/rpc_method.cpp
                   net/tcp_server.h
                   net/tcp_server.cpp
                   net/tcp_server_dispatcher.h
                   net/tcp_server_dispatcher.cpp
                   net/http_socket.h
                   net/http_socket.cpp)
    SET(SRC_SERVICE_XD ${ALL_SRC_SERVICE_XD})
ENDIF()

IF(OS_LINUX)
    SET(SRC_BASE_XD ${ALL_SRC_BASE_XD})
    SET(SRC_NET_XD net/socket_poll.h
                   net/socket_poll.cpp
                   net/socket_epoll.cpp
                   net/socket.h
                   net/socket.cpp
                   net/socket_acceptor.h
                   net/socket_acceptor.cpp
                   net/socket_connection.h
                   net/socket_connection.cpp
                   net/socket_connecter.h
                   net/socket_connecter.cpp
                   net/socket_server.h
                   net/socket_server.cpp
                   net/socket_server_dispatcher.h
                   net/socket_server_dispatcher.cpp
                   net/socket_handler.h
                   net/message.h
                   net/message.cpp
                   net/rpc_message.h
                   net/rpc_message.cpp
                   net/rpc_method.h
                   net/rpc_method.cpp
                   net/tcp_server.h
                   net/tcp_server.cpp
                   net/tcp_server_dispatcher.h
                   net/tcp_server_dispatcher.cpp
                   net/http_socket.h
                   net/http_socket.cpp)
    SET(SRC_SERVICE_XD ${ALL_SRC_SERVICE_XD})
ENDIF()

SET(ALL_SOURCE ${SRC_BASE_XD}
               ${SRC_NET_XD}
               ${SRC_SERVICE_XD})
ADD_LIBRARY(xd ${ALL_SOURCE})
TARGET_LINK_LIBRARIES(xd pthread)
#TARGET_LINK_LIBRARIES(xd protobuf)
