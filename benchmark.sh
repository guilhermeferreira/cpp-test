#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR

source $ROOT_DIR/Config.mk

# Recompile the program using AFL compiler. This compiler instruments the binary
# with fuzzing.
BUILD_DIR=build_benchmark

rm -Rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cmake -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug
cmake --build ${BUILD_DIR} --target benchmark

exit 0
