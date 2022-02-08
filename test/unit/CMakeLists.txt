# package: find external packages
include(FindPkgConfig)

# package: add Google Test library
include(${CMAKE_SOURCE_DIR}/cmake/gtest.cmake)

# compiler: get the list of source code files
get_property(TARGET_SOURCES 
    GLOBAL PROPERTY 
    TARGET_SOURCES_PROP
)

set(TEST_PRE_COMP_HDR test.hxx)

# make: create dynamic header with all unit test
add_custom_target(${TEST_PRE_COMP_HDR}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh ${TEST_PRE_COMP_HDR} ${CMAKE_CURRENT_SOURCE_DIR}
)

# compiler: creates the test binary
add_executable(${TEST}
    ${TARGET_SOURCES}
    ${CMAKE_CURRENT_SOURCE_DIR}/test.cpp
)

if (NOT CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    # compiler: enable Unity Build (one single group)
    set_target_properties(${TEST} PROPERTIES UNITY_BUILD ON)
endif()

# make: add dependency for test binary
add_dependencies(${TEST}
    ${TEST_PRE_COMP_HDR}
)

# linker: link the program against the libraries
target_link_libraries(${TEST}
    ${GTEST_LIBRARIES}
    ${GMOCK_LIBRARIES}
    ${TARGET}_interface
    ${TARGET}_persistence
)

# make: create 'make check' to run unit test
add_custom_target(check
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${TEST} --gtest_repeat=5 --gtest_shuffle
    DEPENDS ${TEST}
)

# linker: link the program against the libraries
target_link_libraries(${TEST}
    -lgcov
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

# cmake: install
install(TARGETS ${TEST} DESTINATION ${PROJECT_SOURCE_DIR})
