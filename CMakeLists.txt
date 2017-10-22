cmake_minimum_required (VERSION 3.0)

set(PROJECT_VERSION_MAJOR 0)
set(PROJECT_VERSION_MINOR 5)
set(PROJECT_VERSION_PATCH 1)
set(PROJECT_VERSION ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}.${PROJECT_VERSION_PATCH})

project (vrb VERSION ${PROJECT_VERSION})

include(GNUInstallDirs)
find_package(PkgConfig)

add_subdirectory(vbuf)
add_subdirectory(vrb)
