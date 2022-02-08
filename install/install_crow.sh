#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR/..

source $ROOT_DIR/Config.mk

#
# Unpack Crow.
#
if [ ! -d $ROOT_DIR/$EXTERNAL_DIR ]; then
	mkdir $ROOT_DIR/$EXTERNAL_DIR
fi
pushd $ROOT_DIR/$EXTERNAL_DIR
if [ ! -f $CROW_FILE ]; then
	wget $CROW_LINK
fi
popd

if [ ! -d $ROOT_DIR/$BUILD_DIR ]; then
	mkdir $ROOT_DIR/$BUILD_DIR
fi
pushd $ROOT_DIR/$BUILD_DIR

#
# Build Crow.
#
if [ ! -d $CROW_DIR ]; then
	mkdir -p $CROW_DIR
else
	echo "The $CROW_DIR is already built!"
fi

popd

exit 0
