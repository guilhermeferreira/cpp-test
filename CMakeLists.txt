# cmake: build options
set(CMAKE_COLOR_MAKEFILE ON)
cmake_minimum_required(VERSION 3.18.4 FATAL_ERROR)

# compiler: use ccache
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM AND (NOT CMAKE_CXX_INCLUDE_WHAT_YOU_USE))
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

if ((NOT CMAKE_C_COMPILER) AND (NOT CMAKE_CXX_COMPILER))
    # cmake: create fuzzable binary
    option(FUZZING "Instrument binary with AFL" OFF)
    if (FUZZING)
        # compiler: force the use of afl compilers
        set(CMAKE_C_COMPILER "afl-gcc")
        set(CMAKE_CXX_COMPILER "afl-g++")
        # Setting AFL_HARDEN automatically adds code hardening options when invoking
        # the downstream compiler. The setting is useful for catching non-crashing
        # memory bugs at the expense of a very slight (sub-5%) performance loss.
        add_compile_options("-DAFL_HARDEN=1")
    else()
        # compiler: force the use of gcc/g++ instead of cc/c++
        set(CMAKE_C_COMPILER "gcc")
        set(CMAKE_CXX_COMPILER "g++")
    endif()
endif()

# cmake: options
option(WINDOW_INTERFACE "Enable Qt window interface" OFF)
option(WEB_INTERFACE "Enable Crow web interface" OFF)
option(COVERAGE "Code coverage" ON)
option(PROFILER "Code profiler" ON)
option(SANITIZER "Address sanitizer" ON)

# project: definitions
project(KVPStorage LANGUAGES C CXX)
execute_process(COMMAND "git tag | tail -n1"
    OUTPUT_VARIABLE VERSION
)
set(VERSION_REVISION 1)
set(TARGET ${PROJECT_NAME})
set(TEST   ${PROJECT_NAME}_test)
set(BM     ${PROJECT_NAME}_bm)

# compiler: add definitions and arguments to the compiler
add_definitions(
    -Wall
    -Wextra
    -Weffc++
    -Wpedantic
    -Wunused
    -Wshadow
    -Wabi=14
    -Winline
    -Wmissing-declarations
    -Wimplicit-fallthrough
    -Wmissing-format-attribute
    -Woverloaded-virtual
    -Wpacked
    -Wredundant-decls
    -Wsign-promo
    -Wstack-protector
    -fstack-protector
    -Wswitch-default
    -Wfloat-equal
    -Wcast-qual
    -Wconversion
)

# cmake: window interface
if (WINDOW_INTERFACE)
    # compiler: add window interface define
    add_definitions(-DWINDOW_INTERFACE)
endif()

# cmake: web interface
if (WEB_INTERFACE)
    # compiler: add web interface define
    add_definitions(-DWEB_INTERFACE)
endif()

# cmake: coverage code
if (COVERAGE)
    # compiler: add code coverage (gcov) options
    add_compile_options(
        --coverage
        -O0
        -fprofile-arcs
        -ftest-coverage
        -ftime-report
    )

    # linker: add code coverage (gcov) options
    add_link_options(
        --coverage
    )
endif()

# cmake: profile the code
if (PROFILER)
    # cmake: find gprof
    find_program(GPROF
        NAMES gprof
        REQUIRED
    )

    # compiler: add profiler (gprof) options
    add_compile_options(
        -pg -g
    )

    # linker: add profiler (gprof) options
    add_link_options(
        -pg
    )
endif()

# compiler: enable sanitizers only non-fuzzing binaries
if (NOT FUZZING AND SANITIZER)
    # compiler: add address sanitizer (ASan)
    add_compile_options(
        -fno-omit-frame-pointer
        -fsanitize=address
    )

    # linker: add address sanitizer (ASan)
    add_link_options(
        -fsanitize=address
    )

    # compiler: add undefined behavior sanitizer (UBSan)
    add_compile_options(
        -fsanitize=undefined
    )

    # linker: add undefined behavior sanitizer (UBSan)
    add_link_options(
        -fsanitize=undefined
    )
endif()

# compiler: set required C++ version
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# compiler: use standard C++ (e.g. -std=c++11 rather than -std=gnu++11)
set(CMAKE_CXX_EXTENSIONS OFF)

# compiler: include directories where the target source code is located
include_directories(src)

# cmake: build the each application in their source directory
add_subdirectory(src)

# cmake: build the test cases
add_subdirectory(test)

# make: create make check to run the test
add_custom_target(dist
    COMMAND ${CMAKE_SOURCE_DIR}/dist.sh ${TARGET}
    DEPENDS ${TARGET}
    WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
)

include(CPack)
