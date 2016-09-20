cmake_minimum_required(VERSION 3.5)
project(Net)


set(SOURCE_FILES
        TransportLayer/net_socket/NetSocket.cpp
        TransportLayer/net_socket/TransportLayerAdapter.cpp
        TransportLayer/net_socket/TcpAdapter.cpp
        TransportLayer/net_socket/Connection.cpp
        ApplicationLayer/DNS/DnsAdapter.cpp
        EventDispatcher/EventDispatcher.cpp
        ../include/EventDispatcher/EventDispatcher.h
        EventDispatcher/IOMultiplexer.cpp
        ../include/EventDispatcher/IOMultiplexer.h

        )

add_library(NetSocket ${SOURCE_FILES})
