#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR

source $ROOT_DIR/Config.mk

# Recompile the program using AFL compiler. This compiler instruments the binary
# with fuzzing.
BUILD_DIR=build_fuzz

rm -Rf ${BUILD_DIR}
mkdir ${BUILD_DIR}
cmake -DFUZZING=On -DCMAKE_BUILD_TYPE=Debug -S . -B ${BUILD_DIR}
make -C ${BUILD_DIR} security

exit 0
