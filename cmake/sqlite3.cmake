# Download and unpack SQLite3 at configure time
include(FetchContent)
FetchContent_Declare(
    sqlite3
    GIT_REPOSITORY https://github.com/sqlite/sqlite.git
    GIT_TAG version-3.34.0
)
FetchContent_MakeAvailable(sqlite3)
