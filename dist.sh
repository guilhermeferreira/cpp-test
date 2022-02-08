#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR

TARGET=$(basename "$1")
VERSION=$(git tag | tail -n1)
FORMAT=zip

#
# Create the distribuition in a shell script instead if cmake because CMake
# add_custom_target() has problems with change directory. If you call
# git-archive from a non-tracked directory (e.g. build), it fails with the
# error "fatal: current working directory is untracked"!
#
echo -n "Creating distribuition file for version ${VERSION}..."
{
    git archive --format=${FORMAT} --prefix=${TARGET}-src/ --output=${BASE_DIR}/dist/${TARGET}-${VERSION}-src.${FORMAT} HEAD
} &> /dev/null
echo "done."

exit 0
