# make: create 'make symbolic' to run symbolic execution
add_custom_target(symbolic
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/test.sh
)
