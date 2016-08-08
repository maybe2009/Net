cmake_minimum_required(VERSION 3.5)
project(Net)

set(SERVER_SOURCE main.cpp)
set(CLIENT_SOURCE client.cpp)
add_executable(myserver ${SERVER_SOURCE})
target_link_libraries(NetSocket)
add_executable(myclient ${CLIENT_SOURCE})
target_link_libraries(NetSocket)
