#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR

source $ROOT_DIR/Config.mk

BUILD_DIR=build_profile

rm -Rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cmake -S . -B ${BUILD_DIR} -DPROFILER=On -DCMAKE_BUILD_TYPE=Debug
cmake --build ${BUILD_DIR} --target profile

exit 0
