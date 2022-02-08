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
# Installs Qt5.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $Qt5_FILE ]; then
	wget $Qt5_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build Qt5.
#
if [ ! -d $Qt5_DIR ]; then
	tar xf $Qt5_FILE
	rm $Qt5_FILE

	pushd $Qt5_DIR

	if [ "$COMPILER" == "" ]; then
		COMPILER=g++;
	fi
	./configure CC=$CC
	make
	make install

	popd

	rm -Rf $Qt5_DIR
else
	echo "The $Qt5_DIR is already built!"
fi

popd

exit 0
