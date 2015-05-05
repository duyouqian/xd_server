# 添加头文件
INCLUDE_DIRECTORIES(../xd/)
INCLUDE_DIRECTORIES(../xd/base/)
INCLUDE_DIRECTORIES(../xd/net/)
INCLUDE_DIRECTORIES(../3rd/)
INCLUDE_DIRECTORIES(../3rd/pb/)
INCLUDE_DIRECTORIES(../3rd/xls/)

ADD_EXECUTABLE(ts_main ts_main.cpp)
TARGET_LINK_LIBRARIES(ts_main xd)

ADD_EXECUTABLE(ts_socket_client ts_socket_client.cpp)
TARGET_LINK_LIBRARIES(ts_socket_client xd)

ADD_EXECUTABLE(ts_socket_server ts_socket_server.cpp)
TARGET_LINK_LIBRARIES(ts_socket_server xd)

