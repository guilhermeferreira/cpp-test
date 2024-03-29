#!/bin/bash

BUILD_TYPE=Debug
# BUILD_TYPE=Release
BUILD_DIR=${BUILD_TYPE}

rm -Rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

# cmake -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
# cmake -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug -G Ninja
# cmake -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld -G Ninja
# cmake -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_C_COMPILER=clang -DCMAKE_EXE_LINKER_FLAGS=-fuse-ld=lld -DSANITIZER=Off -DPROFILER=Off -DCOVERAGE=Off
cmake -S . -B ${BUILD_DIR} \
      -DCMAKE_BUILD_TYPE=${BUILD_DIR} \
      -DCMAKE_CXX_COMPILER=g++ \
      -DCMAKE_C_COMPILER=gcc \
      -DCMAKE_EXPORT_COMPILE_COMMANDS=On \
      -DSANITIZER=Off \
      -DPROFILER=Off \
      -DCOVERAGE=On
time cmake --build ${BUILD_DIR}

exit 0
