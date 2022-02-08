#!/bin/bash

BASE_DIR=$(dirname "$0")
ROOT_DIR=$PWD/$BASE_DIR/../../

source $ROOT_DIR/Config.mk

TARGET_BINARY=$1
INPUT_DIR=$2
OUTPUT_DIR=$3

if [ ! -d $INPUT_DIR ]; then
	MSG="Error: '$INPUT_DIR' directory does not exist."
	echo -e "\e[01;31m$MSG\e[0m" >&2
    exit -1
fi

if [ ! -d $OUTPUT_DIR ]; then
	mkdir $OUTPUT_DIR
fi

AFL_FUZZ=afl-fuzz

# Ignore having crashes misinterpreted as timeouts
export AFL_I_DONT_CARE_ABOUT_MISSING_CRASHES=1
# Ignore miss the short-lived processes spawned by afl-fuzz
export AFL_SKIP_CPUFREQ=1

#
# For programs that take input from a file, use '@@' to mark the location in
# the target's command line where the input file name should be placed. The
# fuzzer will substitute this for you:
#   $ ./afl-fuzz -i testcase_dir -o findings_dir /path/to/program @@
#
$AFL_FUZZ -m 200 -i $INPUT_DIR/database -o $OUTPUT_DIR $TARGET_BINARY --database @@ --ui $INPUT_DIR/script/get.txt

# $AFL_FUZZ -m 200 -i $INPUT_DIR/script -o $OUTPUT_DIR $TARGET_BINARY --database $INPUT_DIR/database/database.ini --ui @@

exit 0
