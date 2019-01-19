set(CMAKE_CXX_STANDARD 17)
project(pbview_test)

message(STATUS "Using Protocol Buffers ${protobuf_VERSION}")

    PROTOBUF_GENERATE_CPP(PROTO_SRCS PROTO_HDRS samples-pb2.proto)
    message(STATUS "PROTO_SRCS: ${PROTO_SRCS}")
    message(STATUS "PROTO_HDRS: ${PROTO_HDRS}")

add_executable(pbview_test CatchMain.cpp BinMessageViewTests.cpp GeneratedViewTests.cpp GeneratedVarTests.cpp ${PROTO_SRCS})
target_link_libraries(pbview_test ${Protobuf_LIBRARIES} ${CONAN_LIBS})

add_executable(pbview_bench bench.cpp ${PROTO_SRCS})
target_link_libraries(pbview_bench ${Protobuf_LIBRARIES} ${CONAN_LIBS} benchmark pthread)

enable_testing()
add_test(NAME pbview_test COMMAND pbview_test)
