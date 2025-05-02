cmake_minimum_required(VERSION 3.5)
# project(OTA_inter LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

# set(CMAKE_AUTOUIC ON)
# set(CMAKE_AUTOMOC ON)
# set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
find_package(Threads REQUIRED)

set(PRJ_NAME OTA_ecu)
# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

#set(PRJ_SRC_GEN_PATH ../src-gen)
#set(PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH ${PRJ_SRC_GEN_PATH}/someip/v0/commonapi)
#FILE(GLOB PRJ_SOMEIP_LIB_SRCS ${PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH}/*cpp)
set(CMAKE_PREFIX_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../install_folder/lib/cmake")

find_package (CommonAPI-SomeIP REQUIRED)
find_package (vsomeip3 REQUIRED)

#for outer communication
set(PRJ_SRC_GEN_PATH_exter ../src-gen-ota-exter)
set(PRJ_SRC_GEN_COMMONAPI_PATH_exter ${PRJ_SRC_GEN_PATH_exter}/core/v0/commonapi)
set(PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH_exter ${PRJ_SRC_GEN_PATH_exter}/someip/v0/commonapi)
#for inner communication
# set(PRJ_SRC_GEN_PATH ../src-gen-ota-inter)
# set(PRJ_SRC_GEN_COMMONAPI_PATH ${PRJ_SRC_GEN_PATH}/core/v0/commonapi)
# set(PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH ${PRJ_SRC_GEN_PATH}/someip/v0/commonapi)
# Application
FILE(GLOB PRJ_PROXY_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Proxy.cpp)
FILE(GLOB PRJ_STUB_GEN_SRCS ${PRJ_SRC_GEN_COMMONAPI_PATH}/*Stub*.cpp)
FILE(GLOB PRJ_STUB_IMPL_SRCS ${PRJ_SRC_COMMONAPI_PATH}/*Stub*.cpp)
 #Boost
#find_package( Boost 1.66 COMPONENTS system thread log REQUIRED )
#include_directories( ${Boost_INCLUDE_DIR} )

FILE(GLOB PRJ_SOMEIP_LIB_SRCS
    #${PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH}/*cpp
    ${PRJ_SRC_GEN_COMMONAPI_SOMEIP_PATH_exter}/*cpp
    )

if(ANDROID)
    add_library(OTA_ecu SHARED
      main.cpp

    )
else()
    add_executable(OTA_ecu
      main.cpp
    )
endif()

# Include directories =
include_directories(
    ../src-gen-ota-inter/
    ../src-gen-ota-exter/
    ../install_folder/include/CommonAPI-3.2
    ../install_folder/include
    ../install_folder/lib
    ../src-gen-ota-inter/core
    ../src-gen-ota-inter/someip
    ../src-gen-ota-exter/core
    ../src-gen-ota-exter/someip
)


# target_compile_definitions(OTA_inter
#   PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
link_directories(
    ../install_folder/lib)
# CommonAPI 라이브러리 찾기
find_library(COMMONAPI_LIB CommonAPI PATHS ../install_folder/lib)
find_library(COMMONAPI_SOMEIP_LIB CommonAPI-SomeIP PATHS ../install_folder/lib)
find_library(VSOMEIP_LIB vsomeip3 PATHS ../install_folder/lib)

message("COMMONAPI_LIB: ${COMMONAPI_LIB}")
message("COMMONAPI_SOMEIP_LIB: ${COMMONAPI_SOMEIP_LIB}")
message("VSOMEIP_LIB: ${VSOMEIP_LIB}")
# 라이브러리 연결
target_link_libraries(OTA_ecu
    PRIVATE
    ${COMMONAPI_LIB}
    ${COMMONAPI_SOMEIP_LIB}
    ${VSOMEIP_LIB}
    Threads::Threads
    )


add_library(${PRJ_NAME}-someip SHARED ${PRJ_SOMEIP_LIB_SRCS})
message("PRJ_SOMEIP_LIB_SRCS = ${PRJ_SOMEIP_LIB_SRCS}")
target_link_libraries(${PRJ_NAME}-someip CommonAPI-SomeIP)
