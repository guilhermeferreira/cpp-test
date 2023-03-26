#!/bin/bash

TEST_DIR=$1
TEST_DST_DIR=$2

echo -n "Copying configuration files..."
rm -Rf $TEST_DST_DIR
mkdir -p $TEST_DST_DIR
cp $TEST_DIR/input/database/database.ini $TEST_DST_DIR
cp $TEST_DIR/input/database/database.db $TEST_DST_DIR
cp $TEST_DIR/input/script/script.txt $TEST_DST_DIR
echo "done."

exit 0
