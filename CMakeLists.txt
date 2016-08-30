cmake_minimum_required(VERSION 3.5)
project(Net)

include_directories(
        ${PROJECT_SOURCE_DIR}/include/TransportLayer
        ${PROJECT_SOURCE_DIR}/include/FixedBuffer
        ${PROJECT_SOURCE_DIR}/include/ApplicationLayer
        ${PROJECT_SOURCE_DIR}/include/Utility
        ${PROJECT_SOURCE_DIR}/include/EventDispatcher
)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g")
set(EXECUTABLE_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${PROJECT_BINARY_DIR}/lib)

add_subdirectory(src)
add_subdirectory(test)