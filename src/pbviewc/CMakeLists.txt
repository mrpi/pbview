set(CMAKE_CXX_STANDARD 17)
project(pbviewc)

message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")

add_executable(pbviewc pbviewc.cpp)
target_link_libraries(pbviewc ${Protobuf_LIBRARIES} protoc protobuf pthread)
