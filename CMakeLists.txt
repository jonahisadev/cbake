# CMake
cmake_minimum_required(VERSION 3.14)
project(cbake)

# C++ Flags
include(GNUInstallDirs)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
option(EnableGlibCxxDebug OFF)

# Sub projects
set(BUILD_CLAR OFF CACHE BOOL "Disable libgit2 tests")
set(BUILD_SHARED_LIBS OFF CACHE BOOL "Force static library build")
add_subdirectory(${PROJECT_SOURCE_DIR}/libs/libgit2)

# Includes
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/cxxopts/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/json/single_include/nlohmann)
include_directories(${PROJECT_SOURCE_DIR}/libs/libgit2/include)
include_directories(${PROJECT_SOURCE_DIR}/libs/ja/include)

# Libraries
link_directories(${PROJECT_SOURCE_DIR}/build/libgit2)

# Source Files
file(GLOB SRC_FILES ${PROJECT_SOURCE_DIR}/src/*.cpp)

# Link
add_executable(cbake ${SRC_FILES})
target_link_libraries(cbake git2)