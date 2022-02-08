#!/bin/bash

./build.sh

BUILD_TYPE=Debug
# BUILD_TYPE=Release
BUILD_DIR=${BUILD_TYPE}

cmake --build ${BUILD_DIR} --target check

exit 0
