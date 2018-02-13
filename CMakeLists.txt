cmake_minimum_required (VERSION 3.1)
project (demo)

# C++11
set (CMAKE_CXX_STANDARD 11)

# Release and debug target compiler flags
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g -DBUILD_DEBUG")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")

# Glob src files
file(GLOB SRC_FILES "src/*.cpp")

# the executable
add_executable(${PROJECT_NAME} ${SRC_FILES})

# libs
INCLUDE(FindPkgConfig)
# SDL2
PKG_SEARCH_MODULE(SDL2 REQUIRED sdl2)
INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES})
