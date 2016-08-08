cmake_minimum_required(VERSION 3.5)
project(Net)

include_directories(
        ${PROJECT_SOURCE_DIR}/include/TransportLayer
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11")
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test/tcp_client)
add_subdirectory(test/tcp_server)