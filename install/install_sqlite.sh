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
# Unpack SQLite.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $SQLITE_FILE ]; then
	wget $SQLITE_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build SQLite.
#
if [ ! -d $SQLITE_DIR ]; then
	tar xf $SQLITE_FILE
	rm $SQLITE_FILE

	pushd $SQLITE_DIR

	if [ "$COMPILER" == "" ]; then
		COMPILER=g++;
	fi
	./configure CC=$CC --prefix=/usr
	make
	make install

	popd

	rm -Rf $SQLITE_DIR
else
	echo "The $SQLITE_DIR is already built!"
fi

popd

exit 0
