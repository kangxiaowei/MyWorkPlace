cmake_minimum_required(VERSION 3.3)
project(flow_server)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

add_library(FlowPipeGen FlowPipe.h FlowPipe.cpp
        FlowPipe_constants.h FlowPipe_constants.cpp
        FlowPipe_types.h FlowPipe_types.cpp)
