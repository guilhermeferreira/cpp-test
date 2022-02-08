#!/bin/bash

TEST_DST_DIR="/tmp/test/symbolic/"

mkdir ${TEST_DST_DIR}

echo -n "Compiling tests..."
ulimit -s unlimited
clang++ -emit-llvm -c -g -DSYMBOLIC=1 -I src test/symbolic/IniFormatSymbolic.cpp -o ${TEST_DST_DIR}/IniFormatSymbolic.bc
clang++ -emit-llvm -c -g -DSYMBOLIC=1 -I src src/persistence/IniFormat.cpp -o ${TEST_DST_DIR}/IniFormat.bc

echo "done."

echo -n "Executing tests..."
klee --libc=uclibc --libcxx --write-cov --write-test-info --emit-all-errors --readable-posix-inputs --posix-runtime --link-llvm-lib=${TEST_DST_DIR}/IniFormat.bc ${TEST_DST_DIR}/IniFormatSymbolic.bc --output-dir=${TEST_DST_DIR}
ktest-tool ${TEST_DST_DIR}/klee-last/test000001.ktest
echo "done."

exit 0
