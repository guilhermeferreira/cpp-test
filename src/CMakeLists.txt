# package: find external packages
include(FindPkgConfig)

# package: add {fmt} library
include(${CMAKE_SOURCE_DIR}/cmake/fmt.cmake)

# cmake: add subdir for interface
add_subdirectory(interface)

# cmake: add subdir for persistence
add_subdirectory(persistence)

# compiler: add the list of source code files to all project
set_property(
    GLOBAL PROPERTY
    TARGET_SOURCES_PROP
    ${CMAKE_CURRENT_SOURCE_DIR}/Core.cpp
)

# compiler: add the path to the binary
set_property(
    GLOBAL PROPERTY
    TARGET_BINARY_PROP
    ${CMAKE_CURRENT_BINARY_DIR}/${TARGET}
)

# compiler: creates the binary
add_executable(${TARGET}
    ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/Core.cpp
)

if (NOT CMAKE_CXX_INCLUDE_WHAT_YOU_USE)
    # compiler: enable Unity Build (one single group)
    set_target_properties(${TARGET} PROPERTIES UNITY_BUILD ON)
    # set_target_properties(${TARGET} PROPERTIES UNITY_BUILD_BATCH_SIZE 8)
endif()

# linker: link the program against the libraries
target_link_libraries(${TARGET}
    ${TARGET}_interface
    ${TARGET}_persistence
    fmt
)

# cmake: install
install(TARGETS ${TARGET} DESTINATION ${PROJECT_SOURCE_DIR})

# cmake: copy Web files
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/WebInterface $<TARGET_FILE_DIR:${PROJECT_NAME}>/WebInterface)
