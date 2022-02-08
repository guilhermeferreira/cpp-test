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
# Unpack Boost.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $BOOST_FILE ]; then
	wget $BOOST_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build Boost.
#
if [ ! -d $BOOST_DIR ]; then
	tar xf $BOOST_FILE
	rm $BOOST_FILE

	pushd $BOOST_DIR

	./bootstrap.sh
	./b2 --prefix=/usr --with-system --with-filesystem --with-thread
	./b2 --prefix=/usr --with-system --with-filesystem --with-thread install

	popd

	rm -Rf $BOOST_DIR
else
	echo "The $BOOST_DIR is already built!"
fi

popd

exit 0
