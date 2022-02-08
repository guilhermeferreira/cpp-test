#!/bin/bash

BUILD_TYPE=Debug
# BUILD_TYPE=Release
BUILD_DIR=${BUILD_TYPE}

rm -Rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cmake -S . -B ${BUILD_DIR} -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=/usr/bin/clang++-7 -DCMAKE_C_COMPILER=/usr/bin/clang-7 -DCMAKE_CXX_INCLUDE_WHAT_YOU_USE=include-what-you-use -DSANITIZER=Off -DPROFILER=Off -DCOVERAGE=Off
cmake --build ${BUILD_DIR} > include-what-you-use.log 2>&1

exit 0
