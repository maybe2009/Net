cmake_minimum_required(VERSION 3.5)
project(Net)

set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(tcp_client)
add_subdirectory(tcp_server)
