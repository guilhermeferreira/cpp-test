# package: find external packages
include(FindPkgConfig)

# package: add Posix Threads library
include(${CMAKE_SOURCE_DIR}/cmake/thread.cmake)

# package: add Boost library
include(${CMAKE_SOURCE_DIR}/cmake/boost.cmake)

# package: add Qt5 library
include(${CMAKE_SOURCE_DIR}/cmake/qt5.cmake)

# compiler: add the list of source code files
set(INTERFACE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Interface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/InterfaceFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/TerminalInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/ScriptInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/WindowInterface.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QtWindow.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/QtWindow.ui
    ${CMAKE_CURRENT_SOURCE_DIR}/WebInterface.cpp
)

# compiler: create the library
add_library(${TARGET}_interface
    ${INTERFACE_SOURCES}
)

if (NOT CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    # compiler: enable Unity Build (two groups)
    set_target_properties(${TARGET}_interface PROPERTIES UNITY_BUILD ON)
    set_target_properties(${TARGET}_interface PROPERTIES UNITY_BUILD_BATCH_SIZE 4)

    # compiler: add precompiled headers (pch)
    target_precompile_headers(${TARGET}_interface
        PRIVATE <cassert>
                <string>
                <iostream>
                ${CMAKE_SOURCE_DIR}/src/strings.hpp
                ${CMAKE_SOURCE_DIR}/src/interface/Interface.hpp
    )
endif()

# linker: link the program against the libraries
target_link_libraries(${TARGET}_interface
    ${Threads_LIBRARIES}
    ${Qt5_LIBRARIES}
    ${Boost_LIBRARIES}
)
