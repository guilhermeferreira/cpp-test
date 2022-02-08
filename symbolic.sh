#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR

source $ROOT_DIR/Config.mk

BUILD_TYPE=Debug
# BUILD_TYPE=Release
BUILD_DIR=${BUILD_TYPE}

rm -Rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

# Compile with CLang instead of GCC because KLEE requires LLVM
cmake -S . \
      -B ${BUILD_DIR} \
      -DCMAKE_BUILD_TYPE=Debug \
      -DCMAKE_C_COMPILER=/usr/local/bin/clang \
      -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ \
      -DCMAKE_CXX_FLAGS="-emit-llvm" \
      -DSYMBOLIC=1 \
      -c \
      -g
cmake --build ${BUILD_DIR} --target symbolic -DSYMBOLIC=1

exit 0
