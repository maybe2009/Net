cmake_minimum_required(VERSION 3.5)
project(Net)

set(CLIENT_SOURCE
        client.cpp)

add_executable(myclient ${CLIENT_SOURCE})
target_link_libraries(myclient NetSocket)

add_executable(test_TcpAdapter test_TcpAdapter.cpp)
target_link_libraries(test_TcpAdapter NetSocket)

add_executable(test_TcpAdapter2 test_TcpAdapter2.cpp)
target_link_libraries(test_TcpAdapter2 NetSocket)