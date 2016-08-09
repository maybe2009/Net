cmake_minimum_required(VERSION 3.5)
project(Net)

set(SERVER_SOURCE main.cpp)
add_executable(myserver ${SERVER_SOURCE})
target_link_libraries(myserver NetSocket)

