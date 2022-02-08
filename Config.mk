BUILD_DIR=dist/
EXTERNAL_DIR=$BUILD_DIR

# Boost (1.68.0)
BOOST_FILE=boost_1_68_0.tar.gz
BOOST_LINK=https://dl.bintray.com/boostorg/release/1.68.0/source/$BOOST_FILE
BOOST_DIR=boost_1_68_0

# Google Test
GTEST_FILE=googletest-release-1.10.0.tar.gz
GTEST_LINK=https://github.com/google/googletest/archive/$GTEST_FILE
GTEST_REG_DIR=googletest-release-reg-1.10.0
GTEST_SAN_DIR=googletest-release-san-1.10.0
GTEST_DIR=googletest-release-1.10.0

# American Fuzzy Lop (AFL)
AFL_FILE=v2.57b.tar.gz
AFL_LINK=https://github.com/google/AFL/archive/$AFL_FILE
AFL_DIR=AFL-2.57b

# Cppcheck
CPPCHECK_FILE=cppcheck-2.3.tar.gz
CPPCHECK_LINK=https://github.com/danmar/cppcheck.git
CPPCHECK_DIR=cppcheck-2.3

# SQLite (3.34.0)
SQLITE_FILE=sqlite-autoconf-3340000.tar.gz
SQLITE_LINK=https://www.sqlite.org/2020/$SQLITE_FILE
SQLITE_DIR=sqlite-autoconf-3340000

# Crow (0.1)
CROW_FILE=crow_all.h
CROW_LINK=https://github.com/ipkn/crow/releases/download/v0.1/$CROW_FILE
CROW_DIR=crow-0.1
