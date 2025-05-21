cmake_minimum_required(VERSION 3.5)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(PRJ_NAME handler_tcp_server)
find_package(Threads REQUIRED)

set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../install_folder/lib/cmake")

find_package (CommonAPI-SomeIP REQUIRED)
find_package (vsomeip3 REQUIRED)

#for outer communication
set(PRJ_SRC_GEN_PATH ../src-gen)
set(PRJ_SRC_GEN_COMMONAPI_PATH ${PRJ_SRC_GEN_PATH}/core/v0/commonapi)
set(PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH ${PRJ_SRC_GEN_PATH}/someip/v0/commonapi)

# Application
FILE(GLOB PRJ_PROXY_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Proxy.cpp)
FILE(GLOB PRJ_STUB_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Stub*.cpp)
FILE(GLOB PRJ_STUB_IMPL_SRCS ${PRJ_SRC_COMMONAPI_PATH}/*Stub*.cpp)

FILE(GLOB PRJ_SOMEIP_LIB_SRCS
    ${PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH}/*cpp
    )

if(ANDROID)
    add_library(handler_tcp_server SHARED
      main.cpp

    )
else()
    add_executable(handler_tcp_server
      main.cpp
      handlerStubImpl.cpp
      handlerStubImpl.hpp
    )
endif()

# Include directories =
include_directories(
    ../src-gen/
    ../install_folder/include/CommonAPI-3.2
    ../install_folder/include
    ../install_folder/lib
    ../src-gen/core
    ../src-gen/someip
)

link_directories(
    ../install_folder/lib)

# CommonAPI 라이브러리 찾기
find_library(COMMONAPI_LIB CommonAPI PATHS ../install_folder/lib)
find_library(COMMONAPI_SOMEIP_LIB CommonAPI-SomeIP PATHS ../install_folder/lib)
find_library(VSOMEIP_LIB vsomeip3 PATHS ../install_folder/lib)

message("COMMONAPI_LIB: ${COMMONAPI_LIB}")
message("COMMONAPI_SOMEIP_LIB: ${COMMONAPI_SOMEIP_LIB}")
message("VSOMEIP_LIB: ${VSOMEIP_LIB}")


include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-lstdc++fs" HAS_STDCXXFS)
if (HAS_STDCXXFS)
    set(FILESYSTEM_LIB stdc++fs)
else()
    set(FILESYSTEM_LIB stdc++fs)
endif()


# 라이브러리 연결
target_link_libraries(handler_tcp_server
    PRIVATE
    ${COMMONAPI_LIB}
    ${COMMONAPI_SOMEIP_LIB}
    ${VSOMEIP_LIB}
    Threads::Threads
    ${FILESYSTEM_LIB}
)

add_library(${PRJ_NAME}-someip SHARED ${PRJ_SOMEIP_LIB_SRCS})
message("PRJ_SOMEIP_LIB_SRCS = ${PRJ_SOMEIP_LIB_SRCS}")
target_link_libraries(${PRJ_NAME}-someip CommonAPI-SomeIP)