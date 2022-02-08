# Qt5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_BUILD_TYPE    "Debug" CACHE "STRING" "" FORCE)
set(CMAKE_PREFIX_PATH   ${Qt5_DIR} CACHE "STRING" "" FORCE)
set(QT_QMAKE_EXECUTABLE ${Qt5_DIR}/bin/qmake CACHE "STRING" "" FORCE)

set(Qt5_LIBRARIES  Qt5::Widgets)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
