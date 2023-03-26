#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR/..

source $ROOT_DIR/Config.mk

UNAME_S=$(uname -s)
if [ $UNAME_S == "Linux" ]; then
  CC=gcc
  CXX=g++
fi

#
# Unpack the Google Benchmark.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $GBENCH_FILE ]; then
	wget $GBENCH_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build normal version of Google Benchmark.
#
if [ ! -d $GBENCH_REG_DIR ]; then
	tar xf $GBENCH_FILE
	rm $GBENCH_FILE

	mv $GBENCH_DIR $GBENCH_REG_DIR

	pushd $GBENCH_REG_DIR

	if [ "$COMPILER" == "" ]; then
		COMPILER=g++;
	fi
	cmake -S . -B ${GBENCH_BUILD_TYPE} \
          -DCMAKE_INSTALL_PREFIX=/usr \
          -DCMAKE_BUILD_TYPE=${GBENCH_BUILD_TYPE} \
          -DBENCHMARK_ENABLE_TESTING=OFF \
          -DBENCHMARK_DOWNLOAD_DEPENDENCIES=OFF \
          -DBUILD_SHARED_LIBS=ON
	cmake --build ${GBENCH_BUILD_TYPE}
	cmake --build ${GBENCH_BUILD_TYPE} --target install

	popd

	rm -Rf $GBENCH_REG_DIR
else
	echo "The $GBENCH_REG_DIR is already built!"
fi

popd

exit 0
