#!/bin/bash

BUILD_TYPE=Debug
# BUILD_TYPE=Release
BUILD_DIR=${BUILD_TYPE}

rm -Rf ${BUILD_DIR}
mkdir ${BUILD_DIR}

cmake -DCOVERAGE=On -DCMAKE_BUILD_TYPE=Debug -S . -B ${BUILD_DIR}
cmake --build ${BUILD_DIR}
cmake --build ${BUILD_DIR} --target check

COV_DIR=$(find ${BUILD_DIR} -name *.gc* | sort -r -n | head -n1)
DATA_DIR=$(dirname $COV_DIR)
COV_INFO=${BUILD_DIR}/coverage.info

lcov -d ${DATA_DIR} -o ${COV_INFO} -b src -c
lcov -d ${DATA_DIR} -o ${COV_INFO} -b src -r ${COV_INFO} \
                                             "/usr/local/*" \
                                             "/usr/include/*" \
                                             "*/test/unit/*"
genhtml ${COV_INFO} -o ${BUILD_DIR}
# firefox ${BUILD_DIR}/index.html


exit 0
