# package: download and unpack Google Test at configure time
include(FetchContent)
FetchContent_Declare(
    googletest
    GIT_REPOSITORY    https://github.com/google/googletest.git
    GIT_TAG           v1.13.0
)
FetchContent_MakeAvailable(googletest)

# package: include external package
include(GoogleTest)

enable_testing()

# compiler: get the list of source code files
get_property(TARGET_SOURCES 
    GLOBAL PROPERTY 
    TARGET_SOURCES_PROP
)

# cmake: get all cpp
file(GLOB TEST_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# compiler: creates the test binary
add_executable(${TEST}
    ${TARGET_SOURCES}
    ${TEST_SOURCES}
)

if (NOT CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    # compiler: enable Unity Build (one single group)
    set_target_properties(${TEST} PROPERTIES UNITY_BUILD ON)
endif()

# compiler: pass destination directory
set(TEST_DST_DIR
    "/tmp/test/unit/"
)
add_definitions(-DTEST_DST_DIR="${TEST_DST_DIR}")

# make: copy configuration files need by the test
add_custom_target(copyfiles
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh ${CMAKE_CURRENT_SOURCE_DIR} ${TEST_DST_DIR}
)

# make: add dependency for test binary
add_dependencies(${TEST}
    copyfiles
)

# linker: link the program against the libraries
target_link_libraries(${TEST}
    PRIVATE ${TARGET}_interface
    PRIVATE ${TARGET}_persistence
    PRIVATE gtest
    PRIVATE gtest_main
    PRIVATE gmock
    PRIVATE gmock_main
    PRIVATE gcov
)

# make: create 'make check' to run unit test
add_custom_target(check
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST} --gtest_repeat=5 --gtest_shuffle
    DEPENDS ${TEST}
)

# make: create 'make coverage' to get code coverage
add_custom_target(coverage
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST} --gtest_filter=-*InterfaceFactoryTestSuite.*:*ScriptInterfaceTestSuite.*:*TerminalInterfaceTestSuite.*
    COMMAND ${CMAKE_SOURCE_DIR}/coverage.sh ${CMAKE_CURRENT_BINARY_DIR}
    DEPENDS ${TEST}
)

# cmake: profile the code
if (PROFILER)
    # make: create 'make profile' to generate code profile
    add_custom_target(profile
        COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST} --gtest_filter=-*InterfaceFactoryTestSuite.*:*ScriptInterfaceTestSuite.*:*TerminalInterfaceTestSuite.*
        COMMAND gprof ${TEST} ${CMAKE_CURRENT_BINARY_DIR}/gmon.out > ${TEST}.gprof
        DEPENDS ${TEST}
    )
endif()

# cmake: 
gtest_discover_tests(${TEST} XML_OUTPUT_DIR ${GTEST_REPORT_DIR})

# cmake: install
install(TARGETS ${TEST} DESTINATION ${PROJECT_SOURCE_DIR})
