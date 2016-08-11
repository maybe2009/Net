cmake_minimum_required(VERSION 3.5)
project(Net)

set(CLIENT_SOURCE
        client.cpp
        test_TcpAdapter.cpp
        test_TcpAdapter2.cpp)

add_executable(myclient ${CLIENT_SOURCE})
target_link_libraries(myclient NetSocket)