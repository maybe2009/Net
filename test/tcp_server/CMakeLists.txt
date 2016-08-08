cmake_minimum_required(VERSION 3.5)
project(Net)

link_directories(
        ${PROJECT_BINARY_DIR}/lib
)
set(SERVER_SOURCE main.cpp)
add_executable(myserver ${SERVER_SOURCE})
target_link_libraries(NetSocket)

