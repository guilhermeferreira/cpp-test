#!/bin/bash

TEST_BIN=$1
TEST_DIR=$2
TEST_DST_DIR="/tmp/test/integration/"

echo -n "Copying configuration files..."
rm -Rf $TEST_DST_DIR
mkdir -p $TEST_DST_DIR
cp $TEST_DIR/input/database/database.ini $TEST_DST_DIR
cp $TEST_DIR/input/database/database.db $TEST_DST_DIR
echo "done."

echo -n "Executing tests..."
expect $TEST_DIR/test.tcl $TEST_BIN
echo "done."

exit 0
