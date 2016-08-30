cmake_minimum_required(VERSION 3.5)
project(Net)

set(SOURCE test_dispatcher.cpp)
add_executable(my_dispatcher ${SOURCE})
target_link_libraries(my_dispatcher NetSocket)

