cmake_minimum_required(VERSION 3.5)
set(CMAKE_CXX_STANDARD 17)
project(pbview LANGUAGES CXX)

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} --std=c++17")

find_package(protobuf REQUIRED)

include_directories(${Protobuf_INCLUDE_DIRS})
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(src)
add_subdirectory(test)
