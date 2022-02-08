# package: find external packages
include(FindPkgConfig)

# compiler: get the path to binary
get_property(TARGET_BINARY
    GLOBAL PROPERTY 
    TARGET_BINARY_PROP
)

# make: create 'make distcheck' to run the integration test
add_custom_target(distcheck
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh ${TARGET_BINARY} ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${TARGET}
)
