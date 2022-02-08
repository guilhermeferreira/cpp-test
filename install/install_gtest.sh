#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR/..

source $ROOT_DIR/Config.mk

UNAME_S=$(uname -s)
if [ $UNAME_S == "Linux" ]; then
#   CC=clang-6.0
#   CXX=clang++-6.0
  CC=gcc
  CXX=g++
fi

#
# Unpack the Google Test.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $GTEST_FILE ]; then
	wget $GTEST_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build normal version of Google Test.
#
if [ ! -d $GTEST_REG_DIR ]; then
	tar xf $GTEST_FILE
	rm $GTEST_FILE

	mv $GTEST_DIR $GTEST_REG_DIR

	pushd $GTEST_REG_DIR

	if [ "$COMPILER" == "" ]; then
		COMPILER=g++;
	fi
	cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX -DBUILD_SHARED_LIBS=ON
	make
	make install

	popd

	rm -Rf $GTEST_REG_DIR
else
	echo "The $GTEST_REG_DIR is already built!"
fi

#
# Build sanitized version of Google Test
#
# if [ ! -d $GTEST_SAN_DIR ]; then
# 	cp $ROOT_DIR/$EXTERNAL_DIR/$GTEST_FILE $ROOT_DIR/$BUILD_DIR/
# 	tar xf $GTEST_FILE
# 	rm $ROOT_DIR/$BUILD_DIR/$GTEST_FILE

# 	mv $GTEST_DIR $GTEST_SAN_DIR

# 	pushd $GTEST_SAN_DIR

# 	if [ "$COMPILER" == "" ]; then
# 		COMPILER=g++;
# 	fi
# 	MSAN_CFLAGS="-fsanitize=memory -stdlib=libc++ -L$ROOT_DIR/$EXTERNAL_DIR/libcxx/libcxx_msan/lib -lc++abi -I$ROOT_DIR/$EXTERNAL_DIR/libcxx/libcxx_msan/include -I$ROOT_DIR/$EXTERNAL_DIR/libcxx/libcxx_msan/include/c++/v1"
# 	cmake . -DCMAKE_BUILD_TYPE=Release -DCMAKE_C_COMPILER=$CC -DCMAKE_CXX_COMPILER=$CXX -DBUILD_SHARED_LIBS=ON -DLLVM_USE_SANITIZER=Memory -DCMAKE_C_FLAGS="$MSAN_CFLAGS" -DCMAKE_CXX_FLAGS="$MSAN_CFLAGS"
# 	make

# 	popd
#
#	rm -Rf $GTEST_SAN_DIR
# else
# 	echo "The $GTEST_SAN_DIR is already built!"
# fi

popd

exit 0
