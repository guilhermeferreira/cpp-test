# package: download and unpack Google Benchmark at configure time
include(FetchContent)

set(FETCHCONTENT_QUIET ON)
set(FETCHCONTENT_UPDATES_DISCONNECTED ON)
set(BUILD_SHARED_LIBS OFF)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    benchmark
    GIT_REPOSITORY    https://github.com/google/benchmark.git
    GIT_TAG           v1.7.1
)
FetchContent_MakeAvailable(benchmark)

# compiler: get the list of benchmark files
file(GLOB BENCHMARK_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# compiler: creates the test binary
add_executable(${BM}
    ${BENCHMARK_SOURCES}
)

if (NOT CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    # compiler: enable Unity Build
    set_target_properties(${BM} PROPERTIES UNITY_BUILD ON)

    # compiler: add precompiled headers (pch)
    target_precompile_headers(${BM}
        PRIVATE <benchmark/benchmark.h>
    )
endif()

# compiler: pass destination directory
set(TEST_DST_DIR
    "/tmp/test/performance/"
)
add_definitions(-DTEST_DST_DIR="${TEST_DST_DIR}")

# linker: link the program against the libraries
target_link_libraries(${BM}
    ${TARGET}_interface
    ${TARGET}_persistence
    benchmark_main
)

# make: create 'make benchmark' to run performance test
add_custom_target(bench
    COMMAND rm -Rf ${TEST_DST_DIR}
    COMMAND mkdir ${TEST_DST_DIR}
    COMMAND cp ${CMAKE_CURRENT_SOURCE_DIR}/input/database/database.* ${TEST_DST_DIR}
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/${BM}
    DEPENDS ${BM}
)

# cmake: install
install(TARGETS ${BM} DESTINATION ${PROJECT_SOURCE_DIR})
