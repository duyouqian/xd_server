FILE(GLOB_RECURSE CURRENT_HEADERS *.h *.cpp)

SOURCE_GROUP("ALL_LIB_SOURCE" FILES ${CURRENT_HEADERS})

ADD_LIBRARY(tinyxml2 ${CURRENT_HEADERS})
