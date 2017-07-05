cmake_minimum_required(VERSION 3.3)
project(flow_server)
include_directories ("${PROJECT_SOURCE_DIR}/gen-cpp")
link_directories("/usr/local/lib/")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake_modules")
add_subdirectory("gen-cpp")

find_package(Boost REQUIRED COMPONENTS system thread timer chrono)

if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREAD ON)

find_package(OpenCV 3 REQUIRED)
find_package(gflags REQUIRED)
message(STATUS "gflags_VERSION: ${gflags_VERSION}")
find_package(glog REQUIRED)
find_library(folly REQUIRED)
find_library(libthrift REQUIRED)

add_library(ThreadPool lib/ThreadPool.h lib/ThreadPool.cpp)
SET_TARGET_PROPERTIES(ThreadPool PROPERTIES LINKER_LANGUAGE CXX)
target_link_libraries(ThreadPool ${Boost_LIBRARIES})

add_library(FlowPipeHandler FlowPipeHandler.h FlowPipeHandler.cpp)
target_link_libraries(FlowPipeHandler ${Boost_LIBRARIES} ThreadPool gflags glog)

set(SOURCE_FILES main.cpp)
add_executable(flow_server ${SOURCE_FILES})
target_link_libraries(flow_server LINK_PUBLIC FlowPipeGen FlowPipeHandler gflags glog thrift ${Boost_LIBRARIES} ThreadPool)
