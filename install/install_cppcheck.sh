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
# Unpack Cppcheck.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $CPPCHECK_FILE ]; then
	wget $CPPCHECK_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build Cppcheck.
#
if [ ! -d $CPPCHECK_DIR ]; then
	tar xf $CPPCHECK_FILE
	rm $CPPCHECK_FILE

	pushd $CPPCHECK_DIR

	if [ "$COMPILER" == "" ]; then
		COMPILER=g++;
	fi
	make
	make install

	popd

	rm -Rf $CPPCHECK_DIR
else
	echo "The $CPPCHECK_DIR is already built!"
fi

popd

exit 0
