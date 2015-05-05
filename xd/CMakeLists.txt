SET(XDBASEDIR .)
FILE(GLOB_RECURSE ALL_SRC_BASE_XD ${XDBASEDIR}/base/*.h ${XDBASEDIR}/base/*.cpp)
FILE(GLOB_RECURSE ALL_SRC_NET_XD ${XDBASEDIR}/net/*.h ${XDBASEDIR}/net/*.cpp)
SOURCE_GROUP("base" FILES ${ALL_SRC_BASE_XD})
SOURCE_GROUP("net" FILES ${ALL_SRC_NET_XD})

IF(APPLE)
    SET(SRC_BASE_XD ${ALL_SRC_BASE_XD})
    SET(SRC_NET_XD net/socket_poll.h
                   net/socket_poll.cpp
                   net/socket_kqueue.cpp
                   net/socket.h
                   net/socket.cpp
                   net/server_socket.h
                   net/server_socket.cpp
                   net/stream_socket.h
                   net/stream_socket.cpp
                   net/http_socket.h
                   net/http_socket.cpp)
ENDIF()

IF(UNIX)
    SET(SRC_BASE_XD ${ALL_SRC_BASE_XD})
    SET(SRC_NET_XD net/socket_poll.h
                   net/socket_poll.cpp
                   net/socket_epoll.cpp
                   net/socket.h
                   net/socket.cpp
                   net/server_socket.h
                   net/server_socket.cpp
                   net/stream_socket.h
                   net/stream_socket.cpp
                   net/http_socket.h
                   net/http_socket.cpp)
ENDIF()

SET(ALL_SOURCE ${SRC_BASE_XD}
               ${SRC_NET_XD})
ADD_LIBRARY(xd ${ALL_SOURCE})
TARGET_LINK_LIBRARIES(xd pthread)
#TARGET_LINK_LIBRARIES(xd protobuf)
