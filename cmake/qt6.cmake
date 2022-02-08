# Qt5
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_BUILD_TYPE    "Debug" CACHE "STRING" "" FORCE)
set(CMAKE_PREFIX_PATH   ${Qt5_DIR} CACHE "STRING" "" FORCE)
set(QT_QMAKE_EXECUTABLE ${Qt5_DIR}/bin/qmake CACHE "STRING" "" FORCE)

set(Qt5_LIBRARIES  Qt5::Widgets)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

if (QT6_FOUND)
    add_subdirectory(Q6)
else()
    add_subdirectory(Q5)
endif()

# ????
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTIUIC ON)
# ????
qt_add_executable(App source)

# Add resource in Qt5
qt5_add_resources(rcc_files resource.qrc)
add_executable(App resource ${rcc_files})
# Add resource in Qt6
qt_add_resource(App app_resources FILES resource.qrc)

# TODO: CMake book (https://crascit.com/professional-cmake/)
# TODO: Qt6 instead of Qt5
# TODO: QML to define GUI
# TODO: Ninja instead of Make. compare build speed. Create graph to show build gains.
# TODO: CMake precompiled header. target_precompiled_headers(). measure build speed before and after. Create graph to show build gains (https://onqtam.com/programming/2019-12-20-pch-unity-cmake-3-16/)
# TODO: Compiler cache. ccache --stat (https://crascit.com/2016/04/09/using-ccache-with-cmake/)
# TODO: CMake Unity builds. set(CMAKE_UNITY_BUILD ON)
# TODO: LLVM Linker (lld) instead of GNU Linker (ld). speed up linker [https://lld.llvm.org/] [episodes 33-34 from KDAB]
# TODO: Add coverage target to QNodeOS.
# TODO: RR recoding executing to debugger. Tool from Mozila.
# TODO: KLEE (https://klee.github.io/tutorials/testing-function/)
# TODO: Perf (https://access.redhat.com/documentation/en-us/red_hat_enterprise_linux/6/html/developer_guide/perf)
# TODO: GDB with QEMU (https://qemu-project.gitlab.io/qemu/system/gdb.html)
# TODO: Coco coverage tool. CoverageBrowser
# TODO: Squish GUI Tester
