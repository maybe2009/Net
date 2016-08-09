cmake_minimum_required(VERSION 3.5)
project(Net)

set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

set(CLIENT_SOURCE client.cpp)
add_executable(myclient ${CLIENT_SOURCE})
target_link_libraries(myclient NetSocket)