# GCC (11.2.0)
FROM gcc:11.2.0 AS dist

WORKDIR /usr/src/

# Packet version
ARG LCOV_VERSION="1.15"
ARG BOOST_VERSION="1_68_0"
ARG CMAKE_VERSION="3.22.3"
ARG GTEST_VERSION="1.11.0"
ARG GBENCH_VERSION="1.6.1"
ARG CPPCHECK_VERSION="2.7"
ARG SQLITE_VERSION="3380100"
ARG CROW_VERSION="0.1"
ARG AFL_VERSION="2.57b"
ARG MINISAT_VERSION="2.2.1"
ARG STP_VERSION="2.3.3"
ARG LLVM_VERSION="9.0.0"
ARG KLEE_ULIB_VERSION="1.2"
ARG KLEE_VERSION="2.2"

# Copy dist/ files
COPY . .

RUN apt-get -qq update
RUN apt-get -qq upgrade

# Tcl/Expect (5.45.4)
RUN apt-get -qq install expect

# Perf (4.19)
RUN apt-get -qq install linux-perf

# Ninja (1.8.2)
RUN apt-get -qq install ninja-build

# Compile Cache - ccache (3.6)
RUN apt-get -qq install ccache

# LCOV (1.15)
RUN git clone https://github.com/linux-test-project/lcov.git && \
    cd lcov && \
    git checkout v${LCOV_VERSION} -b ${LCOV_VERSION} && \
    make PREFIX=/usr/ install && \
    cd ..

# Boost (1.68.0) required by Crow
#                Crow 0.1 works with maximum Boost 1.68.0
# RUN wget https://boostorg.jfrog.io/native/main/release/1.68.0/source/boost_1_68_0.tar.gz && \
RUN tar xf boost_${BOOST_VERSION}.tar.gz && \
    cd boost_${BOOST_VERSION} && \
    ./bootstrap.sh && \
    ./b2 --prefix=/usr --with-system --with-filesystem --with-thread && \
    ./b2 --prefix=/usr --with-system --with-filesystem --with-thread install && \
    cd .. && \
    rm -Rf boost_${BOOST_VERSION}/ && \
    rm boost_${BOOST_VERSION}.tar.gz

# Qt5 (5.11.3)
# RUN apt-get -qq install qt5-default
RUN apt-get -qq install qtbase5-dev
RUN apt-get -qq install qtchooser
RUN apt-get -qq install qt5-qmake
RUN apt-get -qq install qtbase5-dev-tools

# Valgrind (3.14.0)
RUN apt-get -qq install valgrind

# Packages required by KLEE
RUN apt-get -qq install bison
RUN apt-get -qq install build-essential
RUN apt-get -qq install curl
RUN apt-get -qq install doxygen
RUN apt-get -qq install flex
RUN apt-get -qq install git
RUN apt-get -qq install libcap-dev
RUN apt-get -qq install libncurses5-dev
RUN apt-get -qq install libtcmalloc-minimal4
RUN apt-get -qq install libgoogle-perftools-dev
RUN apt-get -qq install libsqlite3-dev
RUN apt-get -qq install gcc-multilib
RUN apt-get -qq install g++-multilib
RUN apt-get -qq install unzip
RUN apt-get -qq install python
RUN apt-get -qq install python3
RUN apt-get -qq install python3-pip
RUN pip3 install lit
RUN pip3 install tabulate
RUN pip3 install wllvm

# CMake (3.22.3)
# NOTE: Command "RUN apt-get -qq install cmake" installs cmake 3.13.4, but we
#       need at least cmake 3.18.4
RUN tar xf cmake-${CMAKE_VERSION}.tar.gz && \
    cd cmake-${CMAKE_VERSION}/ && \
    ./configure && \
    make && \
    make install && \
    cd .. && \
    rm -Rf cmake-${CMAKE_VERSION}/ && \
    rm cmake-${CMAKE_VERSION}.tar.gz

# Google Test (1.11.0)
RUN tar xf googletest-release-${GTEST_VERSION}.tar.gz && \
    cd googletest-release-${GTEST_VERSION}/ && \
    mkdir build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBUILD_SHARED_LIBS=ON -S . -B build && \
    cmake --build build && \
    cmake --build build --target install && \
    cd .. && \
    rm -Rf googletest-release-${GTEST_VERSION}/ && \
    rm googletest-release-${GTEST_VERSION}.tar.gz

# Google Benchmark (1.6.1)
RUN tar xf benchmark-${GBENCH_VERSION}.tar.gz && \
    cd benchmark-${GBENCH_VERSION}/ && \
    cmake -E make_directory build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DBENCHMARK_ENABLE_TESTING=OFF -DBENCHMARK_DOWNLOAD_DEPENDENCIES=OFF -DBUILD_SHARED_LIBS=ON -B build -S . && \
	cmake --build build --target install && \
    cd .. && \
    rm -Rf benchmark-${GBENCH_VERSION}/ && \
    rm benchmark-${GBENCH_VERSION}.tar.gz

# Cppcheck (2.7)
# NOTE: Command "RUN apt-get -qq install cppcheck" installs cppcheck 1.86
RUN tar xf cppcheck-${CPPCHECK_VERSION}.tar.gz && \
    cd cppcheck-${CPPCHECK_VERSION} && \
    mkdir build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DANALYZE_ADDRESS=ON -DANALYZE_DATAFLOW=OFF -DANALYZE_MEMORY=OFF -DANALYZE_THREAD=ON -DANALYZE_UNDEFINED=ON -S . -B build && \
    cmake --build build && \
    cmake --install build && \
    cd .. && \
    rm -Rf cppcheck-${CPPCHECK_VERSION} && \
    rm cppcheck-${CPPCHECK_VERSION}.tar.gz

# SQLite3 (3.38.1)
RUN tar xf sqlite-autoconf-${SQLITE_VERSION}.tar.gz && \
    cd sqlite-autoconf-${SQLITE_VERSION}/ && \
    ./configure --prefix=/usr && \
    make && \
    make install && \
    cd .. && \
    rm -Rf sqlite-autoconf-${SQLITE_VERSION}/ && \
    rm sqlite-autoconf-${SQLITE_VERSION}.tar.gz

# Crow (0.1)
RUN tar xf v${CROW_VERSION}.tar.gz && \
    cd crow-${CROW_VERSION}/ && \
    mkdir build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -S . -B build && \
    cmake --build build && \
    cp build/amalgamate/crow_all.h /usr/include/ && \
    cd .. && \
    rm -Rf crow-${CROW_VERSION}/ && \
    rm v${CROW_VERSION}.tar.gz

# American Fuzzy Lop (2.57b)
RUN tar xf v${AFL_VERSION}.tar.gz && \
    cd AFL-${AFL_VERSION}/ && \
    make && \
    make install && \
    cd .. && \
    rm -Rf AFL-${AFL_VERSION}/ && \
    rm v${AFL_VERSION}.tar.gz

# MiniSAT (2.2.1) required by STP
RUN git clone https://github.com/stp/minisat.git && \
    cd minisat && \
    git checkout -b ${MINISAT_VERSION} releases/${MINISAT_VERSION} && \
    mkdir build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/ -S . -B build && \
    cmake --build build --target install && \
    cd .. && \
    rm -Rf minisat

# STP (2.3.3) required by KLEE
RUN git clone https://github.com/stp/stp.git && \
    cd stp && \
    git checkout -b ${STP_VERSION} tags/${STP_VERSION} && \
    mkdir build && \
    cmake -S . -B build && \
    cmake --build build && \
    cmake --build build --target install && \
    cd .. && \
    rm -Rf stp

# LLVM (9.0.0) required by KLEE
RUN git clone https://github.com/llvm/llvm-project.git && \
    cd llvm-project && \
    git checkout -b ${LLVM_VERSION} llvmorg-${LLVM_VERSION} && \
    mkdir -p llvm/build && \
    cmake -DCMAKE_INSTALL_PREFIX=/usr -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_ENABLE_PROJECTS=lld -S llvm -B llvm/build && \
    cmake --build llvm/build --target install && \
    cd ..

# KLEE-uClibc (1.2) required by KLEE
RUN cd /tmp && \
    git clone https://github.com/klee/klee-uclibc.git && \
    cd klee-uclibc && \
    git checkout -b v${KLEE_ULIB_VERSION} klee_uclibc_v${KLEE_ULIB_VERSION} && \
    ./configure --make-llvm-lib --with-llvm-config=/usr/local/bin/llvm-config && \
    make && \
    make install && \
    cd .. && \
    cd ..

# KLEE (2.2)
RUN git clone https://github.com/klee/klee.git && \
    cd klee && \
    git checkout -b v${KLEE_VERSION} v${KLEE_VERSION} && \
    LLVM_VERSION=9 BASE=/tmp REQUIRES_RTTI=1 DISABLE_ASSERTIONS=1 ENABLE_DEBUG=0 ENABLE_OPTIMIZED=1 ./scripts/build/build.sh libcxx && \
    mkdir build && \
    cd build && \
    cmake \
        -DCMAKE_C_COMPILER=/usr/local/bin/clang \
        -DCMAKE_CXX_COMPILER=/usr/local/bin/clang++ \
        -DENABLE_SOLVER_STP=ON \
        -DENABLE_POSIX_RUNTIME=ON \
        -DENABLE_KLEE_UCLIBC=ON \
        -DKLEE_UCLIBC_PATH=/tmp/klee-uclibc \
        -DENABLE_UNIT_TESTS=OFF \
        -DLLVM_CONFIG_BINARY=/usr/local/bin/llvm-config \
        -DLLVMCC=/usr/local/bin/clang \
        -DLLVMCXX=/usr/local/bin/clang++ \
        -DENABLE_KLEE_LIBCXX=ON \
        -DKLEE_LIBCXX_DIR=/tmp/libc++-install-90/ \
        -DKLEE_LIBCXX_INCLUDE_DIR=/tmp/libc++-install-90/include/c++/v1/ \
        -DENABLE_KLEE_EH_CXX=ON -DKLEE_LIBCXXABI_SRC_DIR=/tmp/llvm-90/libcxxabi/ \
        .. && \
    make && \
    make install && \
    cd .. && \
    cd ..
