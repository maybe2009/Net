cmake_minimum_required(VERSION 3.5)
project(Net)

set(SOURCE_FILES TransportLayer/net_socket/NetSocket.cpp)
add_library(NetSocket ${SOURCE_FILES})
