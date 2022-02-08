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
# Unpacks AFL.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $AFL_FILE ]; then
	wget $AFL_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build AFL.
#
if [ ! -d $AFL_DIR ]; then
	tar xf $AFL_FILE
	rm $AFL_FILE

	pushd $AFL_DIR

	if [ "$COMPILER" == "" ]; then
		COMPILER=g++;
	fi
	make
	make install

	popd

	rm -Rf $AFL_DIR
else
	echo "The $AFL_DIR is already built!"
fi

popd

exit 0
