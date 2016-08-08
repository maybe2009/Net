cmake_minimum_required(VERSION 3.5)
project(Net)

set(SOURCE_FILES main.cpp)
add_executable(mytest ${SOURCE_FILES})
target_link_libraries(NetSocket)
