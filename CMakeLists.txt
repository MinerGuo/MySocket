cmake_minimum_required(VERSION 0.1)

PROJECT (MYSOCKET)
#add_subdirectory(firtexlib)
#add definitions, compiler switches,
ADD_DEFINITIONS(-Werror)
#cmake -DCMAKE_BUILD_TYPE=RELEASE ..
#cmake -DCMAKE_BUILD_TYPE=DEBUG ..
#if you don't want the full compiler output, remove the following line
SET(CMAKE_VERBOSE_MAKEFILE ON)
SET(CMAKE_USE_RELATIVE_PATHS ON)
SET(CMAKE_LIBRARY_OUTPUT_DIRECTORY target/libs)
SET(CMAKE_RUNTIME_OUTPUT_DIRECTORY target)


#INCLUDE_DIRECTORIES(/usr/include/libxml2)

#add source list
AUX_SOURCE_DIRECTORY(lib SRC_MYSOCKET_LIB)

ADD_LIBRARY(mysocket-lib  STATIC  ${SRC_MYSOCKET_LIB})

ADD_EXECUTABLE(00001 00001daytimetcpclii.c )
ADD_EXECUTABLE(00002 00002daytimetcpsrv.c )
ADD_EXECUTABLE(00003 00003byteorder.c )
ADD_EXECUTABLE(00004 00004daytimetcpsrv1.c )
ADD_EXECUTABLE(00005 00005tcpsrv01.c )
ADD_EXECUTABLE(00005tcpcli01 00005tcpcli01.c )

TARGET_LINK_LIBRARIES(00001 mysocket-lib )
TARGET_LINK_LIBRARIES(00002 mysocket-lib )
TARGET_LINK_LIBRARIES(00003 mysocket-lib )
TARGET_LINK_LIBRARIES(00004 mysocket-lib )
TARGET_LINK_LIBRARIES(00005 mysocket-lib )
TARGET_LINK_LIBRARIES(00005tcpcli01 mysocket-lib )



