cmake_minimum_required(VERSION 3.5)
project(Net)

set(CLIENT_SOURCE
        client.cpp)

add_executable(myclient ${CLIENT_SOURCE})
target_link_libraries(myclient NetSocket)

add_executable(test_TcpAdapter2 test_TcpAdapter2.cpp)
target_link_libraries(test_TcpAdapter2 NetSocket)

#set(HEADER
#        ../../include/ApplicationLayer/HttpRequest.h
#)
#set(SOURCE test_DnsAdapter.cpp ../../include/ApplicationLayer/HttpRequest.h)
#set(SOURCE test_DnsAdapter.cpp ${HEADER})
add_executable(test_DnsAdapter test_DnsAdapter)
target_link_libraries(test_DnsAdapter NetSocket)
