#!/bin/bash

#
# Install dependencies
apt install -y clang-7
apt install -y libclang-7-dev

#
# Install include-what-you-use
pushd /tmp
git clone https://github.com/include-what-you-use/include-what-you-use.git
cd include-what-you-use/
git checkout -b 7.0 clang_7.0
mkdir build
cmake -G "Unix Makefiles" -DCMAKE_PREFIX_PATH=/usr/lib/llvm-7 -S . -B build
make -C build
make -C build install
popd

exit 0
