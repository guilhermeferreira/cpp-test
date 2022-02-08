# package: find external packages
include(FindPkgConfig)

# cmake: add unit test subdir
add_subdirectory(unit)

# cmake: add integration test subdir
add_subdirectory(integration)

# cmake: add security test subdir
add_subdirectory(security)

# cmake: add benchmark test subdir
add_subdirectory(benchmark)

# cmake: add symbolic test subdir
add_subdirectory(symbolic)
