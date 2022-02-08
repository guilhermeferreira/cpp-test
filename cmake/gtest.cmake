# Google Test
enable_testing()

# NOTE: GTest is automatically set by find_package(). Thus, there's no need for
#       set(GTEST_LIBRARIES  GTest::GTest GTest::Main)
#       However, GMock must be manually set.
set(GMOCK_LIBRARIES  gmock)

find_package(GTest REQUIRED)
