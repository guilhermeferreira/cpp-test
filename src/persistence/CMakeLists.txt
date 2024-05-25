# package: find external packages
include(FindPkgConfig)

# package: add SQLite library
include(${CMAKE_SOURCE_DIR}/cmake/sqlite3.cmake)

# compiler: add the list of source code files
set(PERSISTENCE_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/Storage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/StorageFactory.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/FileStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/DBStorage.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Format.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/IniFormat.cpp
)

# compiler: create the library
add_library(${TARGET}_persistence
    ${PERSISTENCE_SOURCES}
)

if (NOT CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    # compiler: enable Unity Build (one single group)
    set_target_properties(${TARGET}_persistence PROPERTIES UNITY_BUILD ON)
    set_target_properties(${TARGET}_persistence PROPERTIES UNITY_BUILD_BATCH_SIZE 6)

    # compiler: add precompiled headers (pch)
    target_precompile_headers(${TARGET}_persistence
        PRIVATE <cassert>
                <string>
                <iostream>
                ${CMAKE_SOURCE_DIR}/src/strings.hpp
                ${CMAKE_SOURCE_DIR}/src/persistence/Storage.hpp
    )
endif()

# linker: link the program against the libraries
target_link_libraries(${TARGET}_persistence
    sqlite3
    ${Boost_LIBRARIES}
    -lstdc++fs
)
