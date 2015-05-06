SET(XDBASEDIR .)
FILE(GLOB_RECURSE ALL_SRC_BASE_XD ${XDBASEDIR}/base/*.h ${XDBASEDIR}/base/*.cpp)
FILE(GLOB_RECURSE ALL_SRC_NET_XD ${XDBASEDIR}/net/*.h ${XDBASEDIR}/net/*.cpp)
SOURCE_GROUP("base" FILES ${ALL_SRC_BASE_XD})
SOURCE_GROUP("net" FILES ${ALL_SRC_NET_XD})

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
                   net/socket_server.h
                   net/socket_server.cpp
                   net/tcp_server.h
                   net/tcp_server.cpp
                   net/http_socket.h
                   net/http_socket.cpp)
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
                   net/socket_server.h
                   net/socket_server.cpp
                   net/tcp_server.h
                   net/tcp_server.cpp
                   net/http_socket.h
                   net/http_socket.cpp)
ENDIF()

SET(ALL_SOURCE ${SRC_BASE_XD}
               ${SRC_NET_XD})
ADD_LIBRARY(xd ${ALL_SOURCE})
TARGET_LINK_LIBRARIES(xd pthread)
#TARGET_LINK_LIBRARIES(xd protobuf)
