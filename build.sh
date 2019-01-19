#!/bin/bash

mkdir "build$1"
cd "build$1"
#conan remote add conan-mpusz https://api.bintray.com/conan/mpusz/conan-mpusz
conan install -s build_type=$1 --buil=missing ..
cmake -DCMAKE_BUILD_TYPE=$1 -Dprotobuf_MODULE_COMPATIBLE=1 ..
make pbviewc
./bin/pbviewc --cpp_out=test --proto_path=../test samples-pb2.proto
make -j2
cd ..
